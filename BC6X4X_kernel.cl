const char* kernel_code =
    "__kernel void encrypt(__global char* buffer, int key) {\n"
	
	"if(key==1){\n"
	"buffer[get_global_id(0)]+=13;\n"
	
	"}else if(key==2){\n"
	"buffer[get_global_id(0)]+=get_global_id(0);\n"
    "}\n"
	"}\n"
	
	"__kernel void decrypt(__global char* buffer, int key) {\n"
	
	"if(key==1){\n"
	"buffer[get_global_id(0)]-=13;\n"
	
	"}else if(key==2){\n"
	"buffer[get_global_id(0)]-=get_global_id(0);\n"
	"}\n"
    "}\n"
;