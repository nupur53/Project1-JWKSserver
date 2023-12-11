from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
from argon2 import PasswordHasher
import sqlite3
import uuid
from flask import Flask, request, jsonify

app = Flask(__name__)
DATABASE_FILE = "your_database.db"
AES_KEY = b"your_secret_aes_key"

# Initialize the AES cipher
cipher = Cipher(algorithms.AES(AES_KEY), modes.ECB(), backend=default_backend())
encryptor = cipher.encryptor()
decryptor = cipher.decryptor()

ph = PasswordHasher()

# Database setup
def initialize_database():
    conn = sqlite3.connect(DATABASE_FILE)
    cursor = conn.cursor()

    # Create users table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password_hash TEXT NOT NULL,
            email TEXT UNIQUE,
            date_registered TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            last_login TIMESTAMP
        )
    ''')

    # Create auth_logs table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS auth_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            request_ip TEXT NOT NULL,
            request_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            user_id INTEGER,
            FOREIGN KEY(user_id) REFERENCES users(id)
        )
    ''')

    conn.commit()
    conn.close()

# Encrypt data using AES
def encrypt_data(data):
    return encryptor.update(data.encode()) + encryptor.finalize()

# Decrypt data using AES
def decrypt_data(data):
    return decryptor.update(data) + decryptor.finalize()

# Generate a secure password using UUIDv4
def generate_password():
    return str(uuid.uuid4())

# Hash the password using Argon2
def hash_password(password):
    return ph.hash(password)

# Register a new user
def register_user(username, email):
    password = generate_password()
    hashed_password = hash_password(password)

    # Encrypt username with AES before storing in the database
    encrypted_username = encrypt_data(username)

    conn = sqlite3.connect(DATABASE_FILE)
    cursor = conn.cursor()

    # Insert user details into the users table
    cursor.execute('''
        INSERT INTO users (username, password_hash, email)
        VALUES (?, ?, ?)
    ''', (encrypted_username, hashed_password, email))

    conn.commit()
    conn.close()

    return password

# Log authentication request
def log_authentication_request(request_ip, user_id):
    conn = sqlite3.connect(DATABASE_FILE)
    cursor = conn.cursor()

    # Insert authentication log into the auth_logs table
    cursor.execute('''
        INSERT INTO auth_logs (request_ip, user_id)
        VALUES (?, ?)
    ''', (request_ip, user_id))

    conn.commit()
    conn.close()

@app.route('/register', methods=['POST'])
def register_endpoint():
    data = request.get_json()
    username = data.get('username')
    email = data.get('email')

    if not username or not email:
        return jsonify({'error': 'Invalid request'}), 400

    try:
        initialize_database()
        password = register_user(username, email)
        return jsonify({'password': password}), 201
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/auth', methods=['POST'])
def auth_endpoint():
    data = request.get_json()
    request_ip = request.remote_addr
    username = data.get('username')
    
    if not username:
        return jsonify({'error': 'Invalid request'}), 400

    conn = sqlite3.connect(DATABASE_FILE)
    cursor = conn.cursor()

    # Retrieve user ID from the users table based on the encrypted username
    cursor.execute('''
        SELECT id, username
        FROM users
        WHERE username = ?
    ''', (encrypt_data(username),))

    result = cursor.fetchone()

    if result:
        user_id, encrypted_username = result
        decrypted_username = decrypt_data(encrypted_username).decode()
        log_authentication_request(request_ip, user_id)
        return jsonify({'message': f'Authentication successful for {decrypted_username}'}), 200
    else:
        return jsonify({'error': 'User not found'}), 404

if __name__ == '__main__':
    app.run(debug=True)
