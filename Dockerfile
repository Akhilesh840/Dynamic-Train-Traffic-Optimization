# ----------------------------------------------------
# STAGE 1: Build the C++ Simulation Engine
# ----------------------------------------------------
FROM ubuntu:22.04 AS builder

# Prevent interactive prompts during package install
ENV DEBIAN_FRONTEND=noninteractive

# Install build tools: CMake, G++, Make
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    g++ \
    && rm -rf /var/lib/apt/lists/*

# Set working directory inside container
WORKDIR /app

# Copy CMake configuration, headers, and source files
COPY CMakeLists.txt .
COPY include/ ./include/
COPY src/ ./src/

# Configure and compile using CMake
RUN mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc)

# ----------------------------------------------------
# STAGE 2: Runtime Container (Runs Engine + Serves Web UI)
# ----------------------------------------------------
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install Nginx to serve the dashboard and JSON file
RUN apt-get update && apt-get install -y --no-install-recommends \
    nginx \
    && rm -rf /var/lib/apt/lists/* \
    && sed -i 's/listen 80 default_server;/listen 80 default_server;\n\tlisten 8080 default_server;/' /etc/nginx/sites-available/default

WORKDIR /var/www/html

# Copy compiled executable from builder stage
COPY --from=builder /app/build/TrainEngine /usr/local/bin/TrainEngine

# Copy frontend dashboard
COPY build/dashboard.html /var/www/html/dashboard.html

# Expose HTTP ports (80 and 8080)
EXPOSE 80 8080

# Launch Nginx web server and run the C++ TrainEngine with exec for clean signal handling
CMD ["/bin/bash", "-c", "nginx && cd /var/www/html && exec TrainEngine"]
