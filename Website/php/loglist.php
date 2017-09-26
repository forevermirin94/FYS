<?php 
	include_once 'phpconfig.php';
	
	$mysqli = new mysqli(HOST, USER, PASS, DATABASE);
	if ($mysqli->connect_errno){
		printf("Connection failed: %s\n", $mysqli->connect_error);
		exit();
	}
	$sql = "SELECT * FROM Notifications  ORDER BY Notification_id DESC LIMIT 500"; 	// Met query de notificaties ophalen uit database met een limit van 500
	$result = $mysqli->query($sql);
	$notification_array = array();
	
	  while ($record = $result->fetch_assoc())
		{
			//print_r($record);
			array_push($notification_array, $record);
		}
		
		//omzetten naar een enkele array
		
		print("<table><tbody>");  // tabel openen
		
		for($i = 0; $i <=  sizeOf($notification_array) - 1 ; $i++)
		{
			print( "
					<tr>
						<td width='15%'>" . $notification_array[$i]['Notification_id'] ."</td> 
						<td width='40%'>" . $notification_array[$i]['Notification'] ."</td>
						<td>" . $notification_array[$i]['Timestamp_Notification'] ."</td>
					</tr>
					 
					");
		} // met behulp van for statement elke notificatie in de array afgaan. 
		  // met notificatie wordt deze geprint in een tabel samen met de het ID en de tijd
		
		print("</tbody></table>"); // tabel weer sluiten
?>