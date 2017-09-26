<?php 
	include_once 'phpconfig.php'; //server connection info
	
	$conn = new mysqli(HOST, USER, PASS, DATABASE);
	
	$doorsql = "SELECT `Door` FROM `Setting`"; // gebruik Sql injectie om de hudige deur situatie op te halen (0 = open)
	$result = $conn->query($doorsql);
	$doorstatus = $result->fetch_assoc();       
	
	if($doorstatus[Door] == 0) //check de door status
	{
		$door = '<a type="button" class="deur button" onclick="resetDoor()">Open</a>';
		    // als de deur 0 (open)  is dan wordt de knop weergeven om de deur te restten
	} else {
		$door = '<p>is geopend</P>';
		    // anders wordt alleen weergeven dat de deur open is
	}

	echo ($door);       // pirnt de melding
?>