/**
 * SA:MP JSON plugin version 1.0.0 by Manuel Schnitzer
 */

native json_decode_from_file(const json_filepath[]);
native json_decode(const json_data[]);
native json_get_int(id, const key[]);
native bool:json_get_string(id, const key[], value[], value_length = sizeof(value));
native Float:json_get_float(id, const key[]);
native json_delete(id);
