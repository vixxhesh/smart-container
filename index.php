<!DOCTYPE html>
<html>
<head>
    <title>Smart Container Dashboard</title>
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            background-color: #f0f0f0; /* Set background color */
            margin: 0;
        }
        .container {
            width: 300px;
            height: 200px;
            border: 1px solid #ccc;
            padding: 20px;
            text-align: center;
        }
        .open {
            background-color: #4CAF50;
            color: white;
        }
        .closed {
            background-color: #f44336;
            color: white;
        }
        .safe {
            color: green;
        }
        .unsafe {
            color: red;
        }
    </style>
</head>
<body>
    <h2>Smart Container Dashboard</h2>
    <?php
    $servername = "localhost";
    $dbname = "id21696519_vessel";
    $username = "id21696519_container";
    $password = "C0nt@iner";

    $conn = new mysqli($servername, $username, $password, $dbname);
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    $sql = "SELECT gas_reading, container_closed, Wload, timestamp FROM sensor_data ORDER BY timestamp DESC LIMIT 1";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        $gas_reading = $row["gas_reading"];
        $container_closed = $row["container_closed"];
        $Wload = $row["Wload"];
        $timestamp = $row["timestamp"];

        // echo '<div class="container ' . ($container_closed == "false" ? "closed" : "open") . '">';

        echo '<h3>Container Status:</h3>';
        echo '<p>' . ($container_closed == "close" ? "BOX IS CLOSED" : "BOX IS OPEN") . '</p>';
        echo '<h3>Gas Reading:</h3>';
        echo '<p class="' . ($gas_reading < 1000 ? "unsafe" : "safe") . '">' . $gas_reading . '</p>';
        echo '<h3>Load:</h3>';
        echo '<p>' . $Wload . ' gms</p>';
        echo '<h3>Last Updated:</h3>';
        echo '<p>' . $timestamp . '</p>';
        echo '</div>';
    } else {
        echo "No data available.";
    }

    $conn->close();
    ?>
</body>
</html>
