<?php

//
//(c) Thijs vd Boogaard 2015/2016
//postdata sturen naar een controlpanel om een schakelaar uit te zetten
//Werkt met openZwave-control-panel op port 5555
//

//curl eet alleen arrays met postdata, dus maak een array aan
$postData2 = array();
//deze line zet een schakelaar met device-id 2 uit
$postData2['2-SWITCH BINARY-user-bool-1-0'] = 'false';


//curl_init(); maakt een nieuwe curlfunctie, in dit geval onder $valueConn
//curl_setopt voegt opties toe aan de curlfunctie
$valueConn = curl_init();
curl_setopt($valueConn, CURLOPT_URL, 'http://10.1.1.2:5555/valuepost.html'); //de form waar we naartoe gaan posten
curl_setopt($valueConn, CURLOPT_POST, 34); //de lengte van het constructed postrequest
curl_setopt($valueConn, CURLOPT_POSTFIELDS, $postData2); //de array met waarden die we gaan posten naar de form
curl_setopt($valueConn, CURLOPT_RETURNTRANSFER, 1);
curl_exec($valueConn); //execute de curlfunctie

curl_close($valueConn);
?>