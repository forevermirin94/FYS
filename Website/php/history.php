<?php
	//code van het geschiedenis
	print("<table style='width:100%;'><tbody>");   // begin van tabel
    	
	for($i = 0; $i <= sizeOf($single_cam_array) - 1; $i++)          // gebruik een for statement om alle foto's langs te gaan
	{
	
		$timeStampDisplay = str_replace(' ', '--', $single_cam_array[$i]);      // vervang de spatie met een dubbele streep voor de timestamp
		$timeStamp = str_replace(' ', '_', $single_cam_array[$i]);              // vervang de spatie met een lage streep voor het pad naar de foto
		
		$date = substr($timeStamp, 0, -9);      //maak een aparte variable voor alleen de datum
		$time = substr($timeStamp, -8, 8);      //maak een aparte variable voor alleen de tijd
		
		print("
				<tr>
					<td width='50%'>
						<a href='#' onclick=document.getElementById('lastImage').src='stills/" . $timeStamp . ".jpg'" . ";document.getElementById('lastImageText').innerHTML='" . $timeStampDisplay . "'> " . $time . "
					</td>
					<td width='50%'> " . $date . "</a>
					</td>
				</tr>
		");   
		// per foto word er een rij van een tabel geprint maar daarin de $timestamp. De link verwijst naar javscript dat de src van de afbeelding veranderd naar de desbetreffende afbeelding
	}
	print("</tbody></table>");  //sluit de tabel af
?>