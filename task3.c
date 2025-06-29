#include "task3.h"

void write_blocks_to_file(const char *filename, unsigned char **blocks, size_t block_size, size_t num_blocks) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Eroare: Nu s-a putut deschide fișierul %s pentru scriere.\n", filename);
        return;
    }

    for (int i = 0; i < num_blocks; i++) {
        if (fwrite(blocks[i], sizeof(unsigned char), block_size, file) != block_size) {
            printf("Eroare la scrierea blocului %d în fișier.\n", i + 1);
            fclose(file);
            return;
        }
    }

    fclose(file);
}


unsigned char *P_box(unsigned char *a, size_t a_len) {
    unsigned char *blocks_enc = malloc(a_len * sizeof(unsigned char));
    for (size_t i = 0; i < a_len; i++) {
        blocks_enc[i] = a[(i * (a_len - 1) + 2) % a_len];
    }
    return blocks_enc;
}

unsigned char *XOR(unsigned char *a, size_t a_len, void *iv, size_t iv_len) {
    unsigned char *iv_bytes = (unsigned char *)iv;
    unsigned char *blocks_enc = malloc(a_len * sizeof(unsigned char));
    for (size_t i = 0; i < a_len; i++) {
        blocks_enc[i] = a[i] ^ iv_bytes[i % iv_len];
    }
    return blocks_enc;
}

unsigned char **split_and_pad_blocks(secretariat *db, size_t *len, size_t *block_size) {
    if (!db || !len || !block_size) {
        printf("Eroare: Parametrii invalizi în split_and_pad_blocks.\n");
        return NULL;
    }

    size_t size = sizeof(student) * db->nr_studenti;
    size_t padding = size % 4 ? 4 - size % 4 : 0;

    *len = size + padding;
    *block_size = *len / 4;

    unsigned char *block = calloc(*len, sizeof(unsigned char));
    if (!block) {
        printf("Eroare: Alocare memorie pentru block eșuată.\n");
        return NULL;
    }

    memcpy(block, db->studenti, size);

    unsigned char **blocks = malloc(4 * sizeof(unsigned char *));
    if (!blocks) {
        printf("Eroare: Alocare memorie pentru blocks eșuată.\n");
        free(block);
        return NULL;
    }

    size_t index = 0;
    for (int i = 0; i < 4; i++) {
        blocks[i] = malloc(*block_size * sizeof(unsigned char));
        if (!blocks[i]) {
            printf("Eroare: Alocare memorie pentru blocks[%d] eșuată.\n", i);
            for (int j = 0; j < i; j++) {
                free(blocks[j]);
            }
            free(blocks);
            free(block);
            return NULL;
        }
        memcpy(blocks[i], block + index, *block_size);
        index += *block_size;
    }

    free(block);

    return blocks;
}

void cripteaza_studenti(secretariat *secretariat, void *key, size_t key_len,
                        void *iv, size_t iv_len, char *cale_output) {
    size_t num_blocks = 4, size = 0;
    size_t block_size = 0;

    unsigned char **blocks = split_and_pad_blocks(secretariat, &size, &block_size);
    if (blocks == NULL) {
        printf("Eroare la generarea blocurilor.\n");
        return;
    }

    unsigned char **blocks_enc = malloc(num_blocks * sizeof(unsigned char *));
    if (blocks_enc == NULL) {
        printf("Eroare la alocarea memoriei pentru blocuri criptate.\n");
        for (int i = 0; i < num_blocks; i++) {
            free(blocks[i]);
        }
        free(blocks);
        return;
    }

    blocks_enc[0] = XOR(blocks[0], block_size, iv, iv_len);  // XOR iv

    blocks_enc[0] = XOR(blocks_enc[0], block_size, key, key_len);  // S_box

    blocks_enc[0] = P_box(blocks_enc[0], block_size);  // P_box

    for (int i = 1; i < num_blocks; i++) {
        blocks_enc[i] = XOR(blocks[i], block_size, blocks_enc[i - 1], block_size);  // XOR bloc anterior
        unsigned char *temp = blocks_enc[i];
        blocks_enc[i] = XOR(blocks_enc[i], block_size, key, key_len);  // S_box
        free(temp);

        temp = blocks_enc[i];
        blocks_enc[i] = P_box(blocks_enc[i], block_size);  // P_box
        free(temp);
    }

    write_blocks_to_file(cale_output, blocks_enc, block_size, num_blocks);

    for (int i = 0; i < num_blocks; i++) {
        free(blocks[i]);
        free(blocks_enc[i]);
    }
    free(blocks);
    free(blocks_enc);
}
