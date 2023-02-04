~/Android/Sdk/ndk-bundle/ndk-build

copy_file()
{
	src_file=$1
	dst_dir=$2
	mkdir -p ${dst_dir}
	cp -v ${src_file} ${dst_dir}
}

copy_file ../libs/arm64-v8a/libgti_classify.so ../app/gtiDemo/app/src/main/jniLibs/arm64-v8a/.
copy_file ../libs/armeabi-v7a/libgti_classify.so ../app/gtiDemo/app/src/main/jniLibs/armeabi-v7a/.

copy_file ../libs/arm64-v8a/libgti_classify.so ../../../Lib/Android/arm64-v8a/.
copy_file ../libs/armeabi-v7a/libgti_classify.so ../../../Lib/Android/armeabi-v7a/.