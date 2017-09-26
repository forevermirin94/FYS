<?php
	include_once 'phpconfig.php';
	
	$conn = new mysqli(HOST, USER, PASS, DATABASE);
	if ($conn->connect_errno){
		printf("Connection failed: %s\n", $conn->connect_error);
		exit();
	}
	
	//Een query die informatie haalt van tabel access, employee en notification.
	$sql = "
	SELECT `Access`.`Notification_id`, concat(`Employee`.`First_Name`, ' ' ,`Employee`.`Last_Name`) AS Naam,`Employee`.`Profession` ,TIME(`Notifications`.`Timestamp_Notification`) AS Tijd 
	FROM `Access` 
	INNER JOIN `Employee` 
	ON `Access`.Employee_id = `Employee`.Employee_id 
	INNER JOIN `Notifications` 
	ON `Access`.`Notification_id` = `Notifications`.`Notification_id` 
	ORDER BY `Access`.`Notification_id` DESC";
	$result = $conn->query($sql);
	$Access_array = array();
	
	  while ($record = $result->fetch_assoc())
		{
			//print_r($record);
			array_push($Access_array, $record);
		}
		
		print("<table>
				<tbody>
		");
		
		for($i = 0; $i <=  sizeOf($Access_array) - 1 ; $i++)
		{
			print( "
					<tr width='100%'>
						<td width='20%'>" . $Access_array[$i]['Notification_id'] ."</td>
						<td width='35%'>" . $Access_array[$i]['Naam'] ."</td>
						<td width='30%'>" . $Access_array[$i]['Profession'] ."</td>
						<td width='20%'>" . $Access_array[$i]['Tijd'] ."</td>
					</tr>
					 
					");
		}
		
		print("</tbody>
		</table>");
?>