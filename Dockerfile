FROM debian:bookworm-slim AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential \
       cmake \
       ninja-build \
       python3 \
       python3-pip \
       python3-venv \
       git \
       ca-certificates \
       pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Install Conan 2 into a virtual environment to avoid PEP 668 restrictions
RUN python3 -m venv /opt/conan-venv \
    && /opt/conan-venv/bin/pip install --no-cache-dir --upgrade pip \
    && /opt/conan-venv/bin/pip install --no-cache-dir "conan>=2.2,<3" \
    && ln -s /opt/conan-venv/bin/conan /usr/local/bin/conan

WORKDIR /app
COPY conanfile.py /app

RUN conan profile detect --force
RUN conan install . -of .build -s build_type=Release --build=missing

COPY . /app

RUN conan build . -of .build

FROM debian:bookworm-slim AS runtime

WORKDIR /app
COPY --from=build /app/.build/build/Release/theater /usr/local/bin/theater

CMD ["/usr/local/bin/theater"]
