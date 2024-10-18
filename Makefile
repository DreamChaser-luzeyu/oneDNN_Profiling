.PONY : test clean test_matmul_nosve test_matmul_aarch64sve test_matmul_aarch64sve_gcc13 test_amd64 test_naive_default

clean : 
	rm -rf ./*.tar.gz
	rm -rf ./.build
	rm -rf ./oneDNN_install
	rm -rf ./*.exe
	
test :
	echo $(pwd)

aarch64_nosve.tar.gz : 
	cd $(dirname $0)
	@lsb_release -a | grep jammy \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/download/SHA-a93e56be/build_jammy_arm64nosve.tar.gz -O ./aarch64_nosve.tar.gz \
		|| echo "Not jammy"

	@lsb_release -a | grep noble \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/download/SHA-a93e56be/build_noble_arm64nosve.tar.gz -O ./aarch64_nosve.tar.gz \
		|| echo "Not noble"

aarch64_forcenosimd.tar.gz :
	@lsb_release -a | grep jammy \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/download/SHA-7f2a9e5e/build_jammy_arm64forcenosimd.tar.gz -O ./aarch64_forcenosimd.tar.gz \
		|| echo "Not jammy"

	@lsb_release -a | grep noble \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/download/SHA-7f2a9e5e/build_noble_arm64forcenosimd.tar.gz -O ./aarch64_forcenosimd.tar.gz \
		|| echo "Not noble" 		

aarch64_forcenosve.tar.gz :
	@lsb_release -a | grep jammy \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/download/SHA-7f2a9e5e/build_jammy_arm64forcenosve.tar.gz -O ./aarch64_forcenosve.tar.gz \
		|| echo "Not jammy"

	@lsb_release -a | grep noble \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/download/SHA-7f2a9e5e/build_noble_arm64forcenosve.tar.gz -O ./aarch64_forcenosve.tar.gz \
		|| echo "Not noble" 		

aarch64_sve.tar.gz : 
	@lsb_release -a | grep jammy \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/download/SHA-a526d176/build_jammy_arm64sve.tar.gz -O ./aarch64_sve.tar.gz \
		|| echo "Not jammy"

	@lsb_release -a | grep noble \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/download/SHA-a526d176/build_noble_arm64sve.tar.gz -O ./aarch64_sve.tar.gz \
		|| echo "Not noble"

aarch64_sve_gcc13.tar.gz :		
	@lsb_release -a | grep jammy \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/latest/download/build_jammy_arm64sve_gcc13.tar.gz -O ./aarch64_sve_gcc13.tar.gz \
		|| echo "Not jammy"

	@lsb_release -a | grep noble \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/latest/download/build_noble_arm64sve_gcc13.tar.gz -O ./aarch64_sve_gcc13.tar.gz \
		|| echo "Not noble"

amd64.tar.gz : 
	cd $(dirname $0)
	@lsb_release -a | grep jammy \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/latest/download/build_jammy_amd64.tar.gz -O ./amd64.tar.gz \
		|| echo "Not jammy"

	@lsb_release -a | grep noble \
		&& wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/latest/download/build_noble_amd64.tar.gz -O ./amd64.tar.gz \
		|| echo "Not noble"

test_matmul_aarch64nosve : test aarch64_nosve.tar.gz
	@echo "Uncompressing..."
	tar -zxf ./aarch64_nosve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target demo
	./.build/demo < $(IN_FILE)

test_matmul_aarch64forcenosve : test aarch64_forcenosve.tar.gz
	@echo "Uncompressing..."
	tar -zxf ./aarch64_forcenosve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target demo
	./.build/demo < $(IN_FILE)	

test_matmul_aarch64forcenosimd : test aarch64_forcenosimd.tar.gz
	@echo "Uncompressing..."
	tar -zxf ./aarch64_forcenosimd.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target demo
	./.build/demo < $(IN_FILE)		

test_matmul_aarch64sve : test aarch64_sve.tar.gz
	@echo "Uncompressing..."
	tar -zxf ./aarch64_sve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target demo
	./.build/demo < $(IN_FILE)

test_matmul_aarch64sve_gcc13 : test aarch64_sve_gcc13.tar.gz
	@echo "Uncompressing..."
	tar -zxf ./aarch64_sve_gcc13.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target demo
	./.build/demo < $(IN_FILE)	

test_matmul_amd64 : test amd64.tar.gz
	@echo "Uncompressing..."
	tar -zxf ./amd64.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target demo
	./.build/demo < $(IN_FILE)

test_naive_default : 
	g++ ./profiling_naive.cpp -o ./naive.exe && ./naive.exe && rm ./naive.exe

test_naive_nosve : 
	g++ ./profiling_naive.cpp -O3 -march=armv8-a+nosimd -ggdb3 -o ./naive.exe && ./naive.exe 
#   Should work	
#	qemu-aarch64 -cpu max,sve=off ./naive.exe

test_naive_sve : 
	g++ ./profiling_naive.cpp -O3 -march=armv8-a+sve -fopt-info-vec -ggdb3 -o ./naive-sve.exe && ./naive-sve.exe
#   Illegal instruction is expected here
#	qemu-aarch64 -cpu max,sve=off ./naive-sve.exe
