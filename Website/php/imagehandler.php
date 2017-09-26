<?php 
	include_once 'phpconfig.php'; //server connection info
	session_start();
	// als de sessievariabele login false is, redirect naar de inlogpagina
	if ($_SESSION['login'] == false){
		$_SESSION['errors'] = array("Niet ingelogd!");
		header('Location:index.php');
	}
	
	$mysqli = new mysqli(HOST, USER, PASS, DATABASE);
	if ($mysqli->connect_errno){
		printf("Connection failed: %s\n", $mysqli->connect_error);
		exit();
	}
	
	$sql = "SELECT Timestamp_Photo FROM Camera ORDER BY Timestamp_Photo DESC";  // met sql alle afbeeldingen uit de Datebase halen en aflopend sorteren
	$result = $mysqli->query($sql);
	$cam_array = array();       
	
	while ($record = $result->fetch_assoc())
	{
		//print_r($record);
		array_push($cam_array, $record);
	}
	   // de resultaten van de query vereenvoudigen en in een array zetten

	$single_cam_array =  iterator_to_array(new RecursiveIteratorIterator(new RecursiveArrayIterator($cam_array)), 0);
	    // php zet de resultaten automatisch in een multidimensional array, hier zetten wij het om naar een enkele door de dubbele waarden weg te laten

	$lastpic = (max($single_cam_array));        //aantal foto's bekijken zodat weten welke de laatse is
	$lastpic = str_replace(' ', '_', $lastpic); // string aanpassen zodat de spatie wordt vervangen met een underscore
	$picstring = "stills/{$lastpic}.jpg";       // variable maken voor het pad naar de foto 
	$mysqli->close();
?>