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
        h2 {
            color: #333; /* Dark gray color for headings */
            align-items: center;
            text-align: center;
            text-decoration: solid;
            padding: 20px;
            margin: 20px;
            font-size: 30px;
            text-transform: capitalize;

        }
        .container {
            width: 300px;
            height: 300px;
            border: 1px solid #ccc;
            padding: 20px;
            text-align: center;
        }
        .open {
            background-color: rgba(244, 67, 54, 0.3); /* Light red and transparent */
            color: white;
        }
        .closed {
            background-color: rgba(76, 175, 80, 0.3); /* Light green and transparent */
            color: white;
        }
        .safe {
            color: green;
        }
        .unsafe {
            color: red;
        }
        h3 {
            color: #333; /* Dark gray color for headings */
        }
        p {
            color: #666; /* Medium gray color for paragraph text */
        }
    </style>
</head>
<body>
    <h2>Know the contents!</h2>
    <?php
    /* Add your credentials */
    $servername = "localhost";
    $dbname = ""; 
    $username = "";
    $password = "";

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

        echo '<div class="container ' . ($container_closed == "close" ? "closed" : "open") . '">';
        echo '<h3>Container Status:</h3>';
        echo '<p>' . ($container_closed == "close" ? "BOX IS CLOSED" : "BOX IS OPEN") . '</p>';
        echo '<h3>Gas Reading:</h3>';
        echo '<p class="' . ($gas_reading < 1000 ? "unsafe" : "safe") . '">' . $gas_reading . '</p>';
        echo '<h3>Load:</h3>';
        echo '<p>' . $Wload . ' gms</p>';
        echo '<h3>Last Updated:</h3>';
        echo '<p>' . $timestamp . ' UTC</p>';
        echo '</div>';
    } else {
        echo "No data available.";
    }

    $conn->close();
    ?>
</body>
</html>
