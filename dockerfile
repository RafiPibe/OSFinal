# Set the base image
FROM ubuntu:latest

# Set the working directory
WORKDIR /app

# COPY database.c /app
COPY database.c /app

# Install dependencies and set up the environment
RUN apt-get update && \
    apt-get install -y gcc

# Expose the port
# EXPOSE 8080

# Define the command to compile and run the C program
CMD gcc -o database database.c && ./storage
