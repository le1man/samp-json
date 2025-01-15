# samp-json
A JSON implementation for SA:MP Open.MP Server.

[![CMake on multiple platforms](https://github.com/le1man/samp-json/actions/workflows/build.yml/badge.svg)](https://github.com/le1man/samp-json/actions/workflows/build.yml)

### Using the plugin
There are the following native functions:
```SourcePawn
native json_decode(json_data[]);
native json_get_int(id, key[]);
native bool:json_get_string(id, key[], value[], value_length = sizeof(value));
native Float:json_get_float(id, key[]);
native json_delete(id);
```

can be used as follows:
```Pawn
main(){
    new id_decode = json_decode("{\"key_int\": 1234, \"key_string\": \"hello, world!\", \"key_float\": 1.0000}");
    if(id_decode != 0){
        printf("%d", json_get_int(id_decode, "key_int")); // json_get_int may return -1 if its extraction fails. but -1 may also return as a valid value, make sure your JSON does not use the value -1
        new str[16];
        if(json_get_string(id_decode, "key_stirng", str/*, sizeof(str)*/)){
            printf("%s", str);
        }
        printf("%f", json_get_float(id_decode, "key_float")); // json_get_float has a similar situation, it can return -1.0000 if the value is not found. If you are not sure that you do not have -1 - do not use the validity check of the received data
        json_delete(id_decode); // freeing memory - removing the decoded JSON instance
    }else{
        print("Error decode this JSON!"); // we will output a message about unsuccessful JSON decoding
    }
}
```

The JSON passed to json_decode can be loaded from a file, or from any other resource and passed as a string.