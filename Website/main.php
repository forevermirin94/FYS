<!DOCTYPE html>
<head>
    <title>Web User Interface - Corendon</title>
		<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
	<!-- metatags -->
    <meta name="description" content="">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    
    <!-- favicon-->
    <link rel="shortcut icon" href="get.rekt.info/FYS/favicon.ico" type="image/x-icon">
    <link rel="icon" href="get.rekt.info/FYS/favicon.ico" type="image/x-icon">
    
    <!--    CSS-->
    <link href='http://fonts.googleapis.com/css?family=Source+Sans+Pro:200,400,700' rel='stylesheet' type='text/css'>
    <link rel="stylesheet" href="css/normalize.css">
    <link rel="stylesheet" href="css/main.css">
	
	<!-- php -->
	<?php session_start();
	include ("php/imagehandler.php");
	include ("php/phpconfig.php");
	?>
	

	<script src="js/main.js"></script>
</head>
<body>
	<!-- javascript controle -->
	<noscript>
	 <p class="browsehappy">Om deze site optimaal te gebruiken is het noodzakelijk om Javascript aan te zetten.
	 <!-- target="_blank" wilt zeggen dat je naar een tab of een nieuwe scherm gaat. -->
	 <a href="http://www.enable-javascript.com/nl/" target="_blank">
	 Hier vind je instructies over hoe je Javascript activeert in je web browser</a>.</p>
	</noscript>
	
	<!-- Toelichting van browsehappy: uitvinden wat de laatste versie is van een browser -->
	<!-- Het code hieronder is een conditional comment, Als IE7 = true dan het code uitvoeren If false then wordt het genegeerd -->
    
	<!--[if lt IE 7]>
        <p class="browsehappy">U gebruikt een <strong>verouderde</strong> browser. <a href="http://browsehappy.com/">upgrade uw browser</a> om uw ervaring te verbeteren.</p>
    <![endif]-->
    <div class="wrapper"><!-- Inhoud van het website hier -->		
        <header><!-- inhoud van het kopje -->
			<div class="container clearfix">
				<div class="header">
				<h1>Web User Interface Corendon<a href="php/logout.php" class="button">Logout</a></h1>
				<img alt="Web User Interface Corendon" src="images/corendon.png">
				</div>
            </div>
        </header>
        <div class="container clearfix"><!-- inhoud van camerabeelden en temperatuur -->
            <div class="third widget">
                <h3>Camerabeelden</h3>
				
                <div class="canvas-container" >
                    <img src="<?php echo $picstring; ?>" alt="<?php echo $picstring; ?>"/>
                </div>
				
				<!-- een tekst op het moment dat je een entry doet bij geschiedenis -->
				<!-- <div id="lastImageText"></div> -->
            </div>
			<div class="third widget" style="height:374px;">
                    <h3>Gebouw</h3>
                <div class="canvas-container">
					<div class="gebouw"><!-- huidige temp -->
						<h4>Huidige temperatuur:</h4>
						<!-- strip de celsius uit de temperatuur, hou alleen een integer over -->
						<p><?php include ("php/getTemp.php");
						ob_start();
						include ("php/getTemp.php");
						$tempCur = trim(ob_get_contents(), " °C");
						ob_end_clean();
						?></p>
					</div>
					<div class="gebouw"><!-- kachel -->
						<h4>Kachel status:</h4>
						<a type="button" class="big button" onclick="kachelOn()">Aan</a>
						<a type="button" class="big button" onclick="kachelOff()">Uit</a>
					</div>
					<div class="gebouw"><!-- deur -->
						<h4>Deur:</h4>
						<?php include ("php/door.php");?>
					</div>
					<div class="alert gebouw"><!-- noodsituatie -->
						<h4>Noodsituatie:</h4>
						<?php include ("php/noodsituatie.php"); ?>
					</div>
                </div>
            </div>
            <div class="third widget" style="height:384px;">
                <h3>Temperatuur</h3>
                <div class="canvas-container">
                    <div id="containerCHAR">
						<!--Google AJAX API voor tekenen grafiek!-->
						<!--enige javascript die niet helemaal onderaan het body kan zitten-->
						<script type="text/javascript"
						src="https://www.google.com/jsapi?autoload={
							'modules':[{
							'name':'visualization',
							'version':'1',
							'packages':['corechart']
							}]
						}">
						</script>
						<canvas>
						<script type="text/javascript">

							// Google API voor (line)charts inladen
							google.setOnLoadCallback(drawChart);

							// API-functie. Deze maakt de linechart,
							// voert hem data en tekent hem uit
							function drawChart() 
							{

								// Datatable maken
								var data = new google.visualization.DataTable();
								data.addColumn('datetime', 'Datum');
								data.addColumn('number', 'Temperatuur');
								data.addRows([
									<?php include 'php/chartdata.php'?>
								]);

								// Chartopties
								// TODO: width en height aanpassen naar de box in main.php
								var options = {
									legend: {
										position:'none',	
									},
									chartArea: {
										left:30,
										top:10,
										width:'85%',
										height:'80%',
									},
									vAxis: {
										//format:'none',
										//gridlines: {color: '#333', count: 6},
										ticks: [0,5,10,15,20,25,30,35],
										//viewWindowMode: 'maximized',
										//viewWindow.min: 0,
										//viewWindow.max: 30
									},
									hAxis: {
										format: 'd-M, HH:mm',
									},
									curveType: 'function',
									pointSize:5,
									pointsVisible:'true',
									width: 'width',
									height: 300,
								};

								// Chart tekenen, opties doorgeven
								var chart = new google.visualization.LineChart(document.getElementById('containerCHAR'));
								chart.draw(data, options);
							}
						</script>
						</canvas>
					</div>
                </div>
            </div>
        </div>
        <div class="container clearfix">
            <div class="third widget list_data" style="height:384px;">
                <h3>Geschiedenis</h3>
				<div class="canvas-container">
					<div>
						<table style="width:100%;">
							<thead>
								<tr>	
									<th width='10%' style ="text-align:left;">Tijd</th>
									<th width='10%' style ="text-align:left;">Datum</th>
								</tr>
							</thead>
						</table>
					</div>
					<div style="overflow-y: scroll; height: 275px;">
						<?php include ("php/history.php"); ?>
					</div>
				</div>
            </div>
			<div class="third widget list_data" style="height:384px;">
                <h3>Toegang Registratie</h3>
				<div class="canvas-container">
					<div>
						<table class="tableFourRows" style="width:100%;">
							<thead>
								<tr>	
									<th width='10%'>Nr:</th>
									<th width='35%'>Crewlid</th>
									<th width='30%'>Beroep</th>
									<th width='20%'>Tijd</th>
								</tr>
							</thead>
						</table>
					</div>
					<div style="overflow-y: scroll; height: 275px;">
						<?php include ("php/access.php"); ?>
					</div>
				</div>
            </div>
            <div class="third widget list_data" style="height:384px;">
                <h3>Meldingen</h3>
				<div class="canvas-container">
					<div>
						<table style="width:100%;">
							<thead>
								<tr>	
									<th>Nr:</th>
									<th width='50%'>Omschrijving</th>
									<th width='50%'>Datum - Tijd</th>
								</tr>
							</thead>
						</table>
					</div>
					<div style="overflow-y: scroll; height: 275px;">
						<?php include ("php/loglist.php"); ?>
					</div>
				</div>
            </div>
        </div>
        <div class="push"></div>
    </div>
	<footer>
        <div class="container">
			&copy; 2015-2016 WebUI Corendon &mdash; <a href="contact.html">Contact</a>
		</div>
    </footer>
	
	<!-- javascripts helemaal overaan het body zetten, zodat het javascript elementen sneller kan loaden! -->
	<!--javascript-->
    
	<script type="text/javascript">
	//	$(document).ready( function () {

	//		setTimeout("location.reload(true);", 300000); <!-- elke 20 seconden resetten -->

//			$.get('main.php', function(ret){
		//		$('div class="container clearfix"').php(ret);
	//		});
			
	//		 $('#result').load('php/tempSet.php', function() {
	//		alert('Load was performed.');
	//		});

		//});
    </script>
</body>
</html>
