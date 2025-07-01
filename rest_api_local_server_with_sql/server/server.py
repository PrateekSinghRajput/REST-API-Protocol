import flask
import mysql.connector
from datetime import datetime # Import datetime for proper timestamping

# execute any mysql query
def execute_mysql_query(query, fetch=True):
    # create connection
    connection = None
    cursor = None
    result = None
    try:
        connection = mysql.connector.connect(host="localhost", user="root",
                                            password="1997", database="iot_db")
        
        # get a cursor
        cursor = connection.cursor()

        # execute query
        cursor.execute(query)

        # Commit changes if it's an insert/update/delete query
        if not query.lower().startswith('select'):
            connection.commit()
            
        # Get the result if it's a select query
        if fetch:
            result = cursor.fetchall()

    except mysql.connector.Error as err:
        print(f"Error: {err}")
        # Optionally, roll back in case of an error
        if connection:
            connection.rollback()
    finally:
        # close the cursor and connection
        if cursor:
            cursor.close()
        if connection:
            connection.close()
    
    return result


# create a flask app
app = flask.Flask(__name__)

# route: mapping of path and http method
@app.route("/", methods=["GET"])
def root():
    return "Welcome to the IoT Data Server!"


# Get all temperature and humidity data
@app.route("/data", methods=["GET"])
def get_all_data():
    # Create a query to get both temperature, humidity, and their timestamps
    query = "SELECT temperature, humidity, timestamp FROM sensor_data ORDER BY timestamp DESC"

    # Execute the query and get the result
    result = execute_mysql_query(query)

    # Format the result for JSON response
    formatted_result = []
    for temp, hum, ts in result:
        formatted_result.append({
            "temperature": temp,
            "humidity": hum,
            "timestamp": ts.isoformat() # Convert datetime object to ISO format string
        })
    
    return flask.jsonify(formatted_result)


# Insert new temperature and humidity values
# Changed endpoint to '/add-data' to match ESP32 code
@app.route("/add-data", methods=["POST"])
def insert_sensor_data():
    # Get parameters value sent by client
    temperature = flask.request.form.get('temperature')
    humidity = flask.request.form.get('humidity')

    # Basic validation
    if temperature is None or humidity is None:
        return "Error: Missing temperature or humidity data", 400
    
    try:
        # Convert to float to ensure valid numbers before inserting
        temp_float = float(temperature)
        hum_float = float(humidity)
    except ValueError:
        return "Error: Invalid temperature or humidity format", 400

    # Create an insert query for both temperature and humidity
    # Assuming your table is named 'sensor_data' and has columns 'temperature', 'humidity', 'timestamp'
    # The timestamp will be automatically set by the database if the column is defined as CURRENT_TIMESTAMP
    query = f"INSERT INTO sensor_data (temperature, humidity) VALUES ({temp_float}, {hum_float})"
    print(f"Executing query: {query}")

    # Execute the query (no fetch needed for insert)
    execute_mysql_query(query, fetch=False)

    return "Data inserted successfully", 200 # Return a success message and HTTP 200 OK


# Start the server
if __name__ == "__main__":
    app.run(port=4000, host="0.0.0.0", debug=True)
