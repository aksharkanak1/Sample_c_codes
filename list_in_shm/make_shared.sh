gcc -g -c -Wall -Werror -fpic list_in_shm.c -lpthread
gcc -shared -o liblistInShm.so list_in_shm.o
gcc test_list_in_shm.c -g -L/root/c_code/system-v-ipc liblistInShm.so -lpthread
