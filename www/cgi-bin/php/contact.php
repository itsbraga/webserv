#!/usr/bin/php
<?php
// CGI : écrire les headers directement
echo "Content-Type: text/html\r\n\r\n";

$content_length = getenv('CONTENT_LENGTH');

if ($content_length > 0) {
    $body = file_get_contents('php://stdin');
    parse_str($body, $params);
    
    echo "<h1>Form Received (PHP)</h1>\n";
    echo "<p>Name: " . htmlspecialchars($params['name'] ?? 'N/A') . "</p>\n";
    echo "<p>Email: " . htmlspecialchars($params['email'] ?? 'N/A') . "</p>\n";
    echo "<p>Message: " . htmlspecialchars($params['message'] ?? 'N/A') . "</p>\n";
} else {
    echo "<h1>Error: No data received</h1>\n";
}
?>