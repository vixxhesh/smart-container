<?php
$servername = "localhost";
$dbname = "id21696519_vessel";
$username = "id21696519_container";
$password = "C0nt@iner";

$api_key_value = "12345";
$api_key = $gas_reading = $container_closed = $weight_read  = $value3 = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if ($api_key == $api_key_value) {
        $gas_reading = test_input($_POST["gas_reading"]);
        $container_closed = test_input($_POST["checkContainer"]);
        // $timestamp = test_input($_POST["timestamp"]); // Add timestamp

        $conn = new mysqli($servername, $username, $password, $dbname);
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        $sql = "INSERT INTO sensor_data (gas_reading, container_closed)
        VALUES ('" . $gas_reading . "', '" . $container_closed . "')"; // Adjust SQL query

        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }

        $conn->close();
    } else {
        echo "Wrong API Key";
    }
} else {
    echo "No data posted via HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>
