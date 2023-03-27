__kernel void encrypt(__global char* buffer, int key) {
	if(key==1){
		buffer[get_global_id(0)]+=13;
	}else if(key==2){
		buffer[get_global_id(0)]+=get_global_id(0);
	}
}

__kernel void decrypt(__global char* buffer, int key) {
	if(key==1){
		buffer[get_global_id(0)]-=13;
	}else if(key==2){
		buffer[get_global_id(0)]-=get_global_id(0);
	}
}
