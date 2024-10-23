.PONY : clean matmul_nosve matmul_sve

clean : 
	rm -rf ./*.tar.gz
	rm -rf ./.build
	rm -rf ./oneDNN_install
	rm -rf ./*.exe

oneDNN_nosve.tar.gz : 
	wget https://app.dreamchaser-luzeyu.cn/caalist/d/StaticFiles/study/oneDNN_jammy_aarch64_armv8-a_nosimd_o3_rel_isa_undef.tar.gz -O ./oneDNN_nosve.tar.gz

oneDNN_sve.tar.gz : 
	wget https://app.dreamchaser-luzeyu.cn/caalist/d/StaticFiles/study/oneDNN_jammy_aarch64_armv8-a_sve_o3_rel_auto.tar.gz -O ./oneDNN_sve.tar.gz

oneDNN_asimd.tar.gz : 
	wget https://app.dreamchaser-luzeyu.cn/caalist/d/StaticFiles/study/oneDNN_jammy_aarch64_armv8-a_o3_rel_asimd.tar.gz -O ./oneDNN_asimd.tar.gz

oneDNN_amd64.tar.gz : 
	wget https://github.com/DreamChaser-luzeyu/oneDNN_autobuild/releases/download/SHA-a526d176/build_jammy_amd64.tar.gz -O ./oneDNN_amd64.tar.gz

batchmm_sve : oneDNN_sve.tar.gz
	tar -zxf ./oneDNN_sve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target batchmm
	BUILD_ARCH="sve" ./.build/batchmm < $(IN_FILE)

batchmm_asimd : oneDNN_asimd.tar.gz
	tar -zxf ./oneDNN_asimd.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target batchmm
	BUILD_ARCH="asimd" ./.build/batchmm < $(IN_FILE)

batchmm_nosve : oneDNN_nosve.tar.gz
	tar -zxf ./oneDNN_nosve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target batchmm
	BUILD_ARCH="nosimd" ./.build/batchmm < $(IN_FILE)

batchmm_nodump_sve : oneDNN_sve.tar.gz
	tar -zxf ./oneDNN_sve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target batchmm
	./.build/batchmm < $(IN_FILE)

batchmm_nodump_asimd : oneDNN_asimd.tar.gz
	tar -zxf ./oneDNN_asimd.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target batchmm
	./.build/batchmm < $(IN_FILE)

batchmm_nodump_nosve : oneDNN_nosve.tar.gz
	tar -zxf ./oneDNN_nosve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target batchmm
	./.build/batchmm < $(IN_FILE)

# matsum_sve : oneDNN_sve.tar.gz
# 	tar -zxf ./oneDNN_sve.tar.gz
# 	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
# 	mv ./install ./oneDNN_install
# 	if [ -d "./.build" ]; then rm -rf ./.build; fi
# 	mkdir ./.build
# 	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
# 	cmake --build ./.build --target matsum
# 	./.build/matsum < $(IN_FILE)

# matsum_asimd : oneDNN_asimd.tar.gz
# 	tar -zxf ./oneDNN_asimd.tar.gz
# 	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
# 	mv ./install ./oneDNN_install
# 	if [ -d "./.build" ]; then rm -rf ./.build; fi
# 	mkdir ./.build
# 	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
# 	cmake --build ./.build --target matsum
# 	./.build/matsum < $(IN_FILE)

# matsum_nosve : oneDNN_nosve.tar.gz
# 	tar -zxf ./oneDNN_nosve.tar.gz
# 	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
# 	mv ./install ./oneDNN_install
# 	if [ -d "./.build" ]; then rm -rf ./.build; fi
# 	mkdir ./.build
# 	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
# 	cmake --build ./.build --target matsum
# 	./.build/matsum < $(IN_FILE)

softmax_nosve : oneDNN_nosve.tar.gz
	tar -zxf ./oneDNN_nosve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target softmax
	BUILD_ARCH="nosimd" ./.build/softmax < $(IN_FILE)

softmax_asimd : oneDNN_asimd.tar.gz
	tar -zxf ./oneDNN_asimd.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target softmax
	BUILD_ARCH="asimd" ./.build/softmax < $(IN_FILE)

softmax_sve : oneDNN_sve.tar.gz
	tar -zxf ./oneDNN_sve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target softmax
	BUILD_ARCH="sve" ./.build/softmax < $(IN_FILE)

softmax_nodump_nosve : oneDNN_nosve.tar.gz
	tar -zxf ./oneDNN_nosve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target softmax
	./.build/softmax < $(IN_FILE)

softmax_nodump_asimd : oneDNN_asimd.tar.gz
	tar -zxf ./oneDNN_asimd.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target softmax
	./.build/softmax < $(IN_FILE)

softmax_nodump_sve : oneDNN_sve.tar.gz
	tar -zxf ./oneDNN_sve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target softmax
	./.build/softmax < $(IN_FILE)

relu_nosve : oneDNN_nosve.tar.gz
	tar -zxf ./oneDNN_nosve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target relu
	./.build/relu < $(IN_FILE)

relu_sve : oneDNN_sve.tar.gz
	tar -zxf ./oneDNN_sve.tar.gz
	if [ -d "./oneDNN_install" ]; then rm -rf ./oneDNN_install; fi
	mv ./install ./oneDNN_install
	if [ -d "./.build" ]; then rm -rf ./.build; fi
	mkdir ./.build
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S. -B./.build
	cmake --build ./.build --target relu
	./.build/relu < $(IN_FILE)	
