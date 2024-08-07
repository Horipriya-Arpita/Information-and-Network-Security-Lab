from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.PublicKey import RSA
from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad, unpad
from Crypto.Hash import SHA256
from Crypto.Signature import pkcs1_15
import time
import argparse
import os

def generate_aes_key(length):
    return get_random_bytes(length)

def aes_encrypt(data, key, mode):
    if mode == 'ECB':
        cipher = AES.new(key, AES.MODE_ECB)
        ct_bytes = cipher.encrypt(pad(data, AES.block_size))
        return ct_bytes
    elif mode == 'CFB':
        iv = get_random_bytes(AES.block_size)
        cipher = AES.new(key, AES.MODE_CFB, iv=iv)
        ct_bytes = cipher.encrypt(data)
        return iv + ct_bytes

def aes_decrypt(ct_bytes, key, mode):
    if mode == 'ECB':
        cipher = AES.new(key, AES.MODE_ECB)
        pt = unpad(cipher.decrypt(ct_bytes), AES.block_size)
        return pt
    elif mode == 'CFB':
        iv = ct_bytes[:AES.block_size]
        ct_bytes = ct_bytes[AES.block_size:]
        cipher = AES.new(key, AES.MODE_CFB, iv=iv)
        pt = cipher.decrypt(ct_bytes)
        return pt

def generate_rsa_keys():
    key = RSA.generate(2048)
    private_key = key.export_key()
    public_key = key.publickey().export_key()
    
    with open('private.pem', 'wb') as f:
        f.write(private_key)
    
    with open('public.pem', 'wb') as f:
        f.write(public_key)

def rsa_encrypt(data, public_key_file):
    with open(public_key_file, 'rb') as f:
        public_key = RSA.import_key(f.read())
    cipher = PKCS1_OAEP.new(public_key)
    return cipher.encrypt(data)

def rsa_decrypt(encrypted_data, private_key_file):
    with open(private_key_file, 'rb') as f:
        private_key = RSA.import_key(f.read())
    cipher = PKCS1_OAEP.new(private_key)
    return cipher.decrypt(encrypted_data)

def rsa_sign(data, private_key_file):
    with open(private_key_file, 'rb') as f:
        private_key = RSA.import_key(f.read())
    h = SHA256.new(data)
    signature = pkcs1_15.new(private_key).sign(h)
    return signature

def rsa_verify(data, signature, public_key_file):
    with open(public_key_file, 'rb') as f:
        public_key = RSA.import_key(f.read())
    h = SHA256.new(data)
    try:
        pkcs1_15.new(public_key).verify(h, signature)
        return True
    except (ValueError, TypeError):
        return False

def sha256_hash(data):
    h = SHA256.new(data)
    return h.hexdigest()

def measure_time(func, *args):
    start = time.time()
    result = func(*args)
    end = time.time()
    return result, end - start

def read_file(filename):
    with open(filename, 'rb') as f:
        return f.read()

def write_file(filename, data):
    with open(filename, 'wb') as f:
        f.write(data)

def main():
    parser = argparse.ArgumentParser(description="Cryptography Program")
    parser.add_argument("operation", choices=["aes_encrypt", "aes_decrypt", "rsa_encrypt", "rsa_decrypt", "rsa_sign", "rsa_verify", "sha256"])
    parser.add_argument("input_file", help="Input file")
    parser.add_argument("output_file", help="Output file")
    parser.add_argument("--key_file", help="Key file", required=False)
    parser.add_argument("--mode", choices=["ECB", "CFB"], help="AES mode", required=False)
    args = parser.parse_args()
    
    data = read_file(args.input_file)
    
    if args.operation == "aes_encrypt":
        key = read_file(args.key_file)
        encrypted_data, elapsed_time = measure_time(aes_encrypt, data, key, args.mode)
        write_file(args.output_file, encrypted_data)
        print(f"Encryption took {elapsed_time} seconds")
    elif args.operation == "aes_decrypt":
        key = read_file(args.key_file)
        decrypted_data, elapsed_time = measure_time(aes_decrypt, data, key, args.mode)
        write_file(args.output_file, decrypted_data)
        print(f"Decryption took {elapsed_time} seconds")
    elif args.operation == "rsa_encrypt":
        encrypted_data, elapsed_time = measure_time(rsa_encrypt, data, args.key_file)
        write_file(args.output_file, encrypted_data)
        print(f"RSA Encryption took {elapsed_time} seconds")
    elif args.operation == "rsa_decrypt":
        decrypted_data, elapsed_time = measure_time(rsa_decrypt, data, args.key_file)
        write_file(args.output_file, decrypted_data)
        print(f"RSA Decryption took {elapsed_time} seconds")
    elif args.operation == "rsa_sign":
        signature, elapsed_time = measure_time(rsa_sign, data, args.key_file)
        write_file(args.output_file, signature)
        print(f"RSA Signing took {elapsed_time} seconds")
    elif args.operation == "rsa_verify":
        signature = read_file(args.output_file)
        valid, elapsed_time = measure_time(rsa_verify, data, signature, args.key_file)
        print(f"RSA Verification took {elapsed_time} seconds. Valid: {valid}")
    elif args.operation == "sha256":
        hash_result, elapsed_time = measure_time(sha256_hash, data)
        print(f"SHA-256 Hash: {hash_result}")
        print(f"Hashing took {elapsed_time} seconds")

if __name__ == "__main__":
    main()
