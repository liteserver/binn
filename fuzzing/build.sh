clang -fsanitize=fuzzer,address,undefined -g -O2 fuzz_decoder.c -o fuzz_decoder
clang -fsanitize=fuzzer,address,undefined -g -O2 fuzz_encoder.c -o fuzz_encoder
