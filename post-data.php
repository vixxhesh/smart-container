<?php
$servername = "localhost";
$dbname = "";
$username = "";
$password = "";
/* add your credentials */
$api_key_value = "";
$api_key = $gas_reading = $container_closed = $Wload = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if ($api_key == $api_key_value) {
        $gas_reading = test_input($_POST["gas_reading"]);
        // Check if the container is closed (received as boolean)
        $container_closed = test_input($_POST["container_closed"]);
        $Wload = test_input($_POST["Wload"]);

        $conn = new mysqli($servername, $username, $password, $dbname);
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        $sql = "INSERT INTO sensor_data (gas_reading, container_closed, Wload)
        VALUES ('" . $gas_reading . "', '" . $container_closed . "', '" . $Wload . "')"; // Adjust SQL query

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
