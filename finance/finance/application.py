import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
import numbers

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get data from DB
    portfolio = db.execute("SELECT * FROM stocks WHERE user_id = ?", session["user_id"])
    result = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = result[0]['cash']

    # Pass data to template
    grand_total = cash

    for stock in portfolio:
        price = lookup(stock['symbol'])['price']
        total = round((stock['quantity'] * price), 2)
        stock.update({'price': price, 'total': total})
        grand_total += total

    return render_template("index.html", stocks=portfolio, cash=round(cash, 2), total=round(grand_total, 2))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get data from input form
        symbol = lookup(request.form.get("symbol"))['symbol']
        
        # Validate symbol
        if not symbol:
            return apology("symbol not found", 400)
            
        check = request.form.get("shares")
        
        if not check or not check.isdigit() or check == None:
            return apology("must provide valid number of stocks (integer)", 400)
            
        quantity = int(check)
        
        # Validate quantity
        if not quantity or quantity <= 0:
            return apology("must provide valid number of stocks (integer)", 400)
        
        # Calculating amount of cash for transaction
        price = lookup(symbol)['price']
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
        cash_after = cash - price * quantity

        if cash_after < 0:
            return apology("not enough cash", 400)

        # Insert into stocks table or update row if user already has this stock
        user_stock = db.execute("SELECT quantity FROM stocks WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)

        if not user_stock:
            db.execute("INSERT INTO stocks(user_id, symbol, quantity) VALUES (?, ?, ?)", session["user_id"], symbol, quantity)

        else:
            quantity = quantity + user_stock[0]['quantity']

            db.execute("UPDATE stocks SET quantity = ? WHERE user_id = ? AND symbol = ?", quantity, session["user_id"], symbol)

        # Calculate cash left
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_after, session["user_id"])

        # Register transaction in transactions table
        db.execute("INSERT INTO transactions(user_id, action, symbol, quantity, price, date) VALUES (?, ?, ?, ?, ?, ?)", 
                   session["user_id"], "bought", symbol, quantity, price, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

        # Redirect user to home page
        flash("Transaction successful")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get data from transactions table
    rows = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

    if not rows:
        return apology("no transactions found", 400)

    return render_template("history.html", stocks=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock symbol submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)

        # Pull stock from via API
        stock = lookup(request.form.get("symbol"))

        if not stock:
            return apology("Stock not found", 400)

        return render_template("quote.html", stock=stock)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html", stock="")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password does not match", 400)

        # Ensure username doesn't exists yet
        elif db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username")):
            return apology("username is already taken", 400)

        # hash password
        hash = generate_password_hash(request.form.get("password"))

        # insert into users DB
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), hash)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        quantity = int(request.form.get("shares"))
        symbol = lookup(request.form.get("symbol"))['symbol']
        price = lookup(symbol)['price']

        # Get data from DB
        available = db.execute("SELECT quantity FROM stocks WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)

        # Validate symbol
        if not symbol:
            return apology("symbol not found", 400)

        # Validate quantity
        if not quantity:
            return apology("must provide valid number of stocks owned (integer)", 400)

        # Calculate users quantity of stocks after sell
        quantity_after = available[0]['quantity'] - quantity

        if quantity_after == 0:
            db.execute("DELETE FROM stocks WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        elif quantity_after < 0:
            return apology("you're trying to sell more than you own", 400)
        else:
            # Update with new value
            db.execute("UPDATE stocks SET quantity = ? WHERE user_id = ? AND symbol = ?", 
                       quantity_after, session["user_id"], symbol)

        # Calculate users cash after sell
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
        cash_after = cash + (price * quantity)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_after, session["user_id"])

        # Register transaction in transactions table
        db.execute("INSERT INTO transactions(user_id, action, symbol, quantity, price, date) VALUES (?, ?, ?, ?, ?, ?)", 
                   session["user_id"], "sold", symbol, quantity, price, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

        # Redirect user to home page
        flash("Transaction successful")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Pass from DB to select menu what user has for sell
        rows = db.execute("SELECT symbol, quantity FROM stocks WHERE user_id = ?", session["user_id"])
        stocks = {}
        for row in rows:
            stocks[row['symbol']] = row['quantity']

        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
