<?php

//
//Werkt met openZwave-control-panel op port 5555
//Werking staat beschreven in curlFetch.php in zelfde directory
//

$postData2 = array();
$postData2['2-SWITCH BINARY-user-bool-1-0'] = 'true';

$valueConn = curl_init();
curl_setopt($valueConn, CURLOPT_URL, 'http://10.1.1.2:5555/valuepost.html');
curl_setopt($valueConn, CURLOPT_POST, 34);
curl_setopt($valueConn, CURLOPT_POSTFIELDS, $postData2);
curl_setopt($valueConn, CURLOPT_RETURNTRANSFER, 1);
curl_exec($valueConn);

curl_close($valueConn);
?>