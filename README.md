# samp-json
A JSON implementation for SA:MP Open.MP Server.

[![CMake on multiple platforms](https://github.com/le1man/samp-json/actions/workflows/build.yml/badge.svg)](https://github.com/le1man/samp-json/actions/workflows/build.yml)

### Using the plugin
There are the following native functions:
```SourcePawn
native json_decode_from_file(const json_filepath[]);
native json_decode(const json_data[]);
native json_get_int(id, const key[]);
native bool:json_get_string(id, const key[], value[], value_length = sizeof(value));
native Float:json_get_float(id, const key[]);
native json_delete(id);
```

can be used as follows:
```Pawn
main(){
    new id_decode = json_decode("{\"key_int\": 1234, \"key_string\": \"hello, world!\", \"key_float\": 1.0000}");
    if(id_decode != 0){
        printf("%d", json_get_int(id_decode, "key_int")); // json_get_int may return -1 if its extraction fails. but -1 may also return as a valid value, make sure your JSON does not use the value -1
        new str[16];
        if(json_get_string(id_decode, "key_string", str/*, sizeof(str)*/)){
            printf("%s", str);
        }
        printf("%f", json_get_float(id_decode, "key_float")); // json_get_float has a similar situation, it can return -1.0000 if the value is not found. If you are not sure that you do not have -1 - do not use the validity check of the received data
        json_delete(id_decode); // freeing memory - removing the decoded JSON instance
    }else{
        print("Error decode this JSON!"); // we will output a message about unsuccessful JSON decoding
    }
}
```

There is also a way to open a file.
You can create a test.json file in the scriptfiles folder:
```JSON
{
	"key_int": 1234,
	"key_string": "hello, world!",
	"key_float": 1.0000
}
```

code to read it will be as follows:
```Pawn
main(){
    new id_decode = json_decode_from_file("test.json");
    if(id_decode != 0 || id_decode != -1){
        printf("%d", json_get_int(id_decode, "key_int")); // json_get_int may return -1 if its extraction fails. but -1 may also return as a valid value, make sure your JSON does not use the value -1
        new str[16];
        if(json_get_string(id_decode, "key_string", str/*, sizeof(str)*/)){
            printf("%s", str);
        }
        printf("%f", json_get_float(id_decode, "key_float")); // json_get_float has a similar situation, it can return -1.0000 if the value is not found. If you are not sure that you do not have -1 - do not use the validity check of the received data
        json_delete(id_decode); // freeing memory - removing the decoded JSON instance
    }else if(id_decode == 0){
        print("Error decode this JSON!"); // we will output a message about unsuccessful JSON decoding
    }else if(id_decode == -1){
		print("The file could not be opened, it may not exist");
	}
}
```

The JSON passed to json_decode can be loaded from a file, or from any other resource and passed as a string.