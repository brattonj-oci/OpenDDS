FROM objectcomputing/opendds:release-DDS-3.14

#RUN apt-get update && apt-get -y upgrade && apt-get -y install wget openvpn dnsutils gnupg
RUN apt-get update && apt-get -y install wget openvpn dnsutils gnupg

# Setup Nodejs 10
RUN curl -sL https://deb.nodesource.com/setup_10.x -o setup.sh
RUN chmod +x setup.sh && bash -v setup.sh && rm setup.sh
RUN apt-get -y install nodejs

RUN mkdir -p /dev/net && mknod /dev/net/tun0 c 10 200

# Temp - Soon this will be available via NPM
COPY node-opendds /opt/node-opendds
WORKDIR /opt/node-opendds
RUN npm install && npm install -g node-gyp && node-gyp install --ensure
RUN node-gyp configure build

# These environment variables are needed for node-opendds
# V8_ROOT expects to have /include/node appended to the end
ENV V8_ROOT /usr
ENV NAN_ROOT /opt/node-opendds/node_modules/nan
ENV LD_LIBRARY_PATH /opt/node-opendds/tools/v8stubs:$LD_LIBRARY_PATH
RUN $ACE_ROOT/bin/mwc.pl -type gnuace
RUN make

# Add the node test-app to the image
COPY test_app /opt/test_app
WORKDIR /opt/test_app
RUN $ACE_ROOT/bin/mwc.pl -type gnuace
RUN make

WORKDIR /opt
COPY start.sh /opt/start.sh
# WORKDIR /usr/src/prism-common

# COPY package.json package-lock.json .babelrc /usr/src/prism-common/

# RUN pwd && ls

# RUN npm install --unsafe-perm

# COPY src src

# RUN npm run build

# WORKDIR /usr/src/app

# COPY package.json package-lock.json .babelrc /usr/src/app/

# RUN npm install prism-common && npm install --unsafe-perm

# COPY config config
# COPY src src
# RUN npm run build

# EXPOSE 80

# CMD [ "npm", "start" ]
CMD ["/bin/bash", "/opt/start.sh"]