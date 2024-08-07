from Crypto.PublicKey import RSA
from Crypto.Random import get_random_bytes

def generate_aes_key(length):
    return get_random_bytes(length)

def generate_rsa_keys():
    key = RSA.generate(2048)
    private_key = key.export_key()
    public_key = key.publickey().export_key()
    
    with open('private.pem', 'wb') as f:
        f.write(private_key)
    
    with open('public.pem', 'wb') as f:
        f.write(public_key)

# Generate AES keys
with open('key_128.bin', 'wb') as f:
    f.write(generate_aes_key(16))

with open('key_256.bin', 'wb') as f:
    f.write(generate_aes_key(32))

# Generate RSA keys
generate_rsa_keys()
