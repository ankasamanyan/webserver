# FROM gcc:4.9
# RUN apt-get update
# COPY . /usr/src/webserv
# WORKDIR /usr/src/webserv
# EXPOSE 2000
# CMD ["make", "run"]
# # ENTRYPOINT ["make", "run"]

# Use a buildpack-deps image with Debian (buster) as the base
FROM buildpack-deps:buster

# Install Python 3 and related packages
RUN apt-get update && \
    apt-get install -y python3 python3-pip

# Install build-essential for C++ compiler and make
RUN apt-get install -y build-essential

# Set the working directory
WORKDIR /usr/src/webserv

# Define the command to run your application
CMD ["make", "run"]
