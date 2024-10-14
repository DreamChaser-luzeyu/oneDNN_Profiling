.PONY : test clean test_matmul_nosve test_amd64

clean : 
	rm -rf ./*.tar.gz
	rm -rf ./.build
	rm -rf ./oneDNN_install
	
test :
	echo $(pwd)

aarch64_nosve.tar.gz : 
	cd $(dirname $0)
	@lsb_release -a | grep jammy \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/latest/download/build_jammy_arm64nosve.tar.gz -O ./aarch64_nosve.tar.gz \
		|| echo "Not jammy"

	@lsb_release -a | grep nobel \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/latest/download/build_noble_arm64nosve.tar.gz -O ./aarch64_nosve.tar.gz \
		|| echo "Not nobel"

aarch64_sve.tar.gz : 
	cd $(dirname $0)
	@lsb_release -a | grep jammy \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/latest/download/build_jammy_arm64sve.tar.gz -O ./aarch64_sve.tar.gz \
		|| echo "Not jammy"

	@lsb_release -a | grep nobel \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/latest/download/build_noble_arm64sve.tar.gz -O ./aarch64_sve.tar.gz \
		|| echo "Not nobel"

amd64.tar.gz : 
	cd $(dirname $0)
	@lsb_release -a | grep jammy \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/latest/download/build_jammy_amd64.tar.gz -O ./amd64.tar.gz \
		|| echo "Not jammy"

	@lsb_release -a | grep nobel \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/latest/download/build_noble_amd64.tar.gz -O ./amd64.tar.gz \
		|| echo "Not nobel"

test_matmul_aarch64nosve : test aarch64_nosve.tar.gz
	@echo "Uncompressing..."
	tar -zxf ./aarch64_nosve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -S. -B./.build
	cmake --build ./.build --target demo
	./.build/demo < $(IN_FILE)

test_matmul_aarch64sve : test aarch64_sve.tar.gz
	@echo "Uncompressing..."
	tar -zxf ./aarch64_sve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -S. -B./.build
	cmake --build ./.build --target demo
	./.build/demo < $(IN_FILE)

test_matmul_amd64 : test amd64.tar.gz
	@echo "Uncompressing..."
	tar -zxf ./amd64.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -S. -B./.build
	cmake --build ./.build --target demo
	./.build/demo < $(IN_FILE)
