from flask import Flask, render_template, request, redirect, url_for, session
import subprocess
import os
import re
import mysql.connector
from werkzeug.security import generate_password_hash, check_password_hash

app = Flask(__name__)
app.secret_key = 'your_secret_key'

db = mysql.connector.connect(
    host="localhost",
    user="root",
    password="123456789",
    database="lexical_db"
)
cursor = db.cursor()

@app.route('/', methods=['GET', 'POST'])
def index():
    if 'user_id' not in session:
        return redirect(url_for('login'))

    output = ''
    code = ''

    if request.method == 'POST':
        action = request.form.get('action')

        if action == 'clear':
            code = ''
            output = ''
        elif action == 'analyze':
            code = request.form['code']

            with open('input.c', 'w') as f:
                f.write(code + "\n$")

            exe_path = os.path.join(os.getcwd(), "lexical.exe")

            if not os.path.exists(exe_path):
                output = "Error: lexical.exe not found in the current directory."
            else:
                try:
                    result = subprocess.run([exe_path], capture_output=True, text=True, timeout=10)
                    output = result.stdout

                    cursor.execute(
                        "INSERT INTO analysis (user_id, input_code, tokens) VALUES (%s, %s, %s)",
                        (session['user_id'], code, output)
                    )
                    db.commit()

                except subprocess.TimeoutExpired:
                    output = "Error: lexical analyzer timed out."
                except Exception as e:
                    output = f"Error: {e}"

    return render_template('index.html', output=output, code=code)

@app.route('/signup', methods=['GET', 'POST'])
def signup():
    msg = ''
    if request.method == 'POST':
        first_name = request.form['first_name']
        last_name = request.form['last_name']
        email = request.form['email']
        password = request.form['password']

        if not re.match(r'^[a-zA-Z0-9._]+@gmail\.com$', email):
            msg = 'Only valid Gmail addresses are allowed!'
            return render_template('signup.html', msg=msg)

        cursor.execute("SELECT id FROM users WHERE email = %s", (email,))
        account = cursor.fetchone()

        if account:
            msg = 'Email already registered!'
        else:
            hashed_password = generate_password_hash(password)
            cursor.execute(
                "INSERT INTO users (first_name, last_name, email, password, plain_password) VALUES (%s, %s, %s, %s, %s)",
                (first_name, last_name, email, hashed_password, password)
            )
            db.commit()
            msg = 'Signup successful! Please login.'
            return redirect(url_for('login'))

    return render_template('signup.html', msg=msg)

@app.route('/login', methods=['GET', 'POST'])
def login():
    msg = ''
    if request.method == 'POST':
        email = request.form['email']
        password = request.form['password']

        cursor.execute("SELECT id, password FROM users WHERE email = %s", (email,))
        account = cursor.fetchone()

        if account and check_password_hash(account[1], password):
            session['user_id'] = account[0]
            return redirect(url_for('index'))
        else:
            msg = 'Invalid email or password!'

    return render_template('login.html', msg=msg)

@app.route('/logout')
def logout():
    session.clear()
    return redirect(url_for('login'))

if __name__ == '__main__':
    app.run(debug=True)
