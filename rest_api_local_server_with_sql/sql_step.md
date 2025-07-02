pip3 install flask
python3 -m flask --version
pip3 install mysql-connector-python
python3 server.py


//Step-1

mysql -u root -p -h localhost


//Step-2

SHOW DATABASES;

//Step-3

CREATE DATABASE sensor;

//Step-4

USE sensor;


//Step-5;

CREATE TABLE sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    temperature FLOAT NOT NULL,
    humidity FLOAT NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

//Step-6;

SHOW TABLES;

//Step-7;

SELECT * FROM sensor_data;


//Step-8; run in direct terminal not sql

watch -n 2 "mysql -u root -p'1997' -h localhost sensor -e \"SELECT * FROM sensor_data ORDER BY timestamp DESC LIMIT 5;\""

//only 5 data display in sql format
mysql -u root -p'1997' -h localhost sensor -e "SELECT * FROM sensor_data ORDER BY timestamp DESC LIMIT 5;"

//Step-9;

EXIT;


//Step-10;
//Delete the database

DROP DATABASE iot_db;

//Delete the sensordata
DELETE FROM sensor_data;
DELETE FROM sensor_data WHERE timestamp < NOW() - INTERVAL 7 DAY;
DELETE FROM sensor_data WHERE temperature < 10.0 OR temperature > 40.0;
DELETE FROM sensor_data WHERE id = 123;

