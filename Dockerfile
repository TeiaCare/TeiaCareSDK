ARG BASE_IMAGE=ubuntu:22.04
FROM ${BASE_IMAGE} AS base

ARG C_COMPILER=gcc
ARG CXX_COMPILER=g++
ARG COMPILER_VERSION=12
ARG BUILD_TYPE=Release
ARG NINJA_STATUS='[%s/%t %p :: %e] '

RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    python3.10 python3-pip python3.10-venv \
    ${C_COMPILER}-${COMPILER_VERSION} \
    ${CXX_COMPILER}-${COMPILER_VERSION}

ENV CC=${C_COMPILER}-${COMPILER_VERSION}
ENV CXX=${CXX_COMPILER}-${COMPILER_VERSION}

WORKDIR /teiacare_sdk
COPY /scripts/conan scripts/conan
COPY /scripts/requirements.txt scripts/requirements.txt
RUN python3 -m venv .venv
RUN . /teiacare_sdk/.venv/bin/activate
RUN python3 -m pip install -r scripts/requirements.txt --no-cache-dir --disable-pip-version-check

###############################################################################
## unit tests - coverage
## docker build --target tests . -t sdk:tests
## docker build --target tests-export --output type=local,dest=export/tests . 
###############################################################################
FROM base AS tests
ARG BUILD_TYPE
COPY /sdk/tests/conanfile.txt sdk/tests/conanfile.txt
RUN python3 scripts/conan/profile.py ${C_COMPILER} ${COMPILER_VERSION}
RUN python3 scripts/conan/install.py ${BUILD_TYPE} ${C_COMPILER}${COMPILER_VERSION} -d sdk/tests
COPY . .

RUN python3 scripts/cmake/configure.py ${BUILD_TYPE} --warnings --unit_tests --coverage
RUN python3 scripts/cmake/build.py
RUN python3 scripts/cmake/install.py
RUN python3 scripts/tools/run_unit_tests.py ./install/tests/teiacare_sdk_unit_tests
RUN python3 scripts/tools/run_coverage.py ${C_COMPILER} ${COMPILER_VERSION}

## tests-export
FROM scratch as tests-export
COPY --from=tests /teiacare_sdk/results/ .

###############################################################################
## unit tests - address sanitizer
## docker build --target address-sanitizer . -t sdk:address-sanitizer
## docker build --target address-sanitizer-export --output type=local,dest=export/address-sanitizer . 
###############################################################################
FROM base AS address-sanitizer
ARG BUILD_TYPE
COPY /sdk/tests/conanfile.txt sdk/tests/conanfile.txt
RUN python3 scripts/conan/profile.py ${C_COMPILER} ${COMPILER_VERSION}
RUN python3 scripts/conan/install.py ${BUILD_TYPE} ${C_COMPILER}${COMPILER_VERSION} -d sdk/tests
COPY . .

RUN python3 scripts/cmake/configure.py ${BUILD_TYPE} --warnings --unit_tests --address_sanitizer
RUN python3 scripts/cmake/build.py
RUN python3 scripts/cmake/install.py
RUN python3 scripts/tools/run_sanitizer.py ./install/tests/teiacare_sdk_unit_tests --gtest_filter=-test_uuid* --address_sanitizer

## address-sanitizer-export
FROM scratch as address-sanitizer-export
COPY --from=tests /teiacare_sdk/results/ .

###############################################################################
## unit tests - thread sanitizer
## docker build --target thread-sanitizer . -t sdk:thread-sanitizer
## docker build --target thread-sanitizer-export --output type=local,dest=export/thread-sanitizer . 
###############################################################################
FROM base AS thread-sanitizer
ARG BUILD_TYPE
COPY /sdk/tests/conanfile.txt sdk/tests/conanfile.txt
RUN python3 scripts/conan/profile.py ${C_COMPILER} ${COMPILER_VERSION}
RUN python3 scripts/conan/install.py ${BUILD_TYPE} ${C_COMPILER}${COMPILER_VERSION} -d sdk/tests
COPY . .

RUN python3 scripts/cmake/configure.py ${BUILD_TYPE} --warnings --unit_tests --thread_sanitizer
RUN python3 scripts/cmake/build.py
RUN python3 scripts/cmake/install.py
RUN python3 scripts/tools/run_sanitizer.py ./install/tests/teiacare_sdk_unit_tests --gtest_filter=-test_uuid* --thread_sanitizer

## thread-sanitizer-export
FROM scratch as thread-sanitizer-export
COPY --from=tests /teiacare_sdk/results/ .

###############################################################################
## examples
## docker build --target examples . -t sdk:examples
###############################################################################
FROM base AS examples
ARG BUILD_TYPE
COPY /sdk/examples/conanfile.txt sdk/examples/conanfile.txt
RUN python3 scripts/conan/profile.py ${C_COMPILER} ${COMPILER_VERSION}
RUN python3 scripts/conan/install.py ${BUILD_TYPE} ${C_COMPILER}${COMPILER_VERSION} -d sdk/examples
COPY . .

RUN python3 scripts/cmake/configure.py ${BUILD_TYPE} --warnings --examples
RUN python3 scripts/cmake/build.py
RUN python3 scripts/cmake/install.py
RUN python3 scripts/tools/run_examples.py install/examples

###############################################################################
## benchmarks
## docker build --target benchmarks . -t sdk:benchmarks
## docker build --target benchmarks-export --output type=local,dest=export/benchmarks . 
###############################################################################
FROM base AS benchmarks
ARG BUILD_TYPE
COPY /sdk/benchmarks/conanfile.txt sdk/benchmarks/conanfile.txt
RUN python3 scripts/conan/profile.py ${C_COMPILER} ${COMPILER_VERSION}
RUN python3 scripts/conan/install.py ${BUILD_TYPE} ${C_COMPILER}${COMPILER_VERSION} -d sdk/benchmarks
COPY . .

RUN python3 scripts/cmake/configure.py ${BUILD_TYPE} --warnings --benchmarks
RUN python3 scripts/cmake/build.py
RUN python3 scripts/cmake/install.py
RUN python3 scripts/tools/run_benchmarks.py install/benchmarks/teiacare_sdk_benchmarks

## benchmarks-export
FROM scratch as benchmarks-export
COPY --from=benchmarks /teiacare_sdk/results/ .

###############################################################################
## valgrind
## docker build --target valgrind . -t sdk:valgrind
## docker build --target valgrind-export --output type=local,dest=export/valgrind . 
###############################################################################
FROM base AS valgrind
ARG BUILD_TYPE=Debug
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    valgrind

COPY /sdk/tests/conanfile.txt sdk/tests/conanfile.txt
RUN python3 scripts/conan/profile.py ${C_COMPILER} ${COMPILER_VERSION}
RUN python3 scripts/conan/install.py ${BUILD_TYPE} ${C_COMPILER}${COMPILER_VERSION} -d sdk/tests
COPY . .

RUN python3 scripts/cmake/configure.py ${BUILD_TYPE} --warnings --unit_tests
RUN python3 scripts/cmake/build.py
RUN python3 scripts/cmake/install.py
RUN python3 scripts/tools/run_valgrind.py install/tests/teiacare_sdk_unit_tests --gtest_filter=-test_uuid*:test_rate_limiter* --memcheck
RUN python3 scripts/tools/run_valgrind.py install/tests/teiacare_sdk_unit_tests --gtest_filter=-test_uuid*:test_rate_limiter* --callgrind 

## valgrind-export
FROM scratch as valgrind-export
COPY --from=valgrind /teiacare_sdk/results/ .

###############################################################################
## docs
## docker build --target docs . -t sdk:docs
## docker build --target docs-export --output type=local,dest=export/docs . 
###############################################################################
FROM base AS docs
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    doxygen graphviz
COPY . .

RUN python3 scripts/cmake/configure.py ${BUILD_TYPE} --docs
RUN python3 scripts/cmake/build.py
RUN python3 scripts/cmake/install.py

## docs-export
FROM scratch as docs-export
COPY --from=docs /teiacare_sdk/install/docs/ .