run:
	cmake-build-debug/tomcat_bouncer /home/phiro/CLionProjects/tomcat-bouncer/test
build_container:
	cd infra/local &&  docker buildx build --load --platform linux/amd64 -t lottonz/tomcat-bouncer-on-filedrop:0.0.1 -f Dockerfile ../..
	cd infra/local && docker scout quickview
run_container:
	docker run -it \
       --platform linux/amd64 \
	   --mount type=bind,source=$(shell pwd)/listener,target=/listener\
	   --mount type=bind,source=$(shell pwd)/app,target=/app \
	    lottonz/tomcat-bouncer-on-filedrop:0.0.1

