FROM gcc:14.1.0-bookworm as builder
RUN mkdir /src && mkdir /app
WORKDIR /src
COPY . .
CMD ["gcc", "-o", "/app/tomcat-bouncer", "tomcat-bouncer.c"]

