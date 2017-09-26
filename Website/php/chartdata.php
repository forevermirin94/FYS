<?php
// phpconfig.php definiëert de constanten voor de databaseconnectie
		include_once 'phpconfig.php';
		
		// verbinding maken, query opbouwen, resultaat opslaan in $tempArray
		$mysqli = new mysqli(HOST, USER, PASS, DATABASE);
		$sql = "SELECT * FROM `Temperature` ORDER BY `Timestamp` DESC ";
		$result = $mysqli->query($sql);
		$tempArray = array();
								
		// we willen alle resultaten, dus we maken van $tempArray een associative array
		while ($record = $result->fetch_assoc())
		{
			array_push($tempArray, $record);
		}
		
		// voor de laatste 12 entries in de table
		// 12 entries van 5 minuten = afgelopen uur
		for($i=0; $i <= 11; $i++)
		{
			// datetime parsen;
			// stap 1, opslaan in een tijdelijke variabele
			$datetime = strtotime($tempArray[$i]["Timestamp"]);
			
			// stap 2, opnieuw formatten zodat javascript er wat mee kan
			// 1993-12-30T00:00:00+0100 = 30 december 1993, 00:00:00, GMT + 1
			$dateFormat = date("Y-m-d\TH:i:s\+0100", $datetime);
			
			// we willen ook de temperatuur, dus haal de bijbehorende waarden uit het assoc array
			$temperature = $tempArray[$i]["Temperature"];
			
			// bouw de javascript data block voor de chartplotter
			// met 2 tabjes zodat de source formatting ook netjes is
			echo "\t\t[new Date('$dateFormat'), $temperature]";
			
			// nog niet bij de laatste entry? voeg dan een kommaatje toe
			if ($i != count($tempArray) - 1)
			{
				echo ",\n";
			}
		}
		// klaar? dan een nieuwline voor de opmaak
		
		echo "\n";
?>