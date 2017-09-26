<!DOCTYPE html>
<html>
    <head>
		<?php session_start();?>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Login</title>
		<!-- Script -->
		<script src="//code.jquery.com/jquery-1.10.2.js"></script>
		<script src="//code.jquery.com/ui/1.11.4/jquery-ui.js"></script>
		<script src="js/index.js"></script>
		
		<!-- css -->
        <link rel="stylesheet" href="css/index.css">
		<link rel="stylesheet" href="//code.jquery.com/ui/1.11.4/themes/smoothness/jquery-ui.css">
    </head>
    <body>
		<div class="login_screen_outer">
			<div class="login_screen_middle">
				<div class="login_screen_inner">
					<center>
						<!-- logo corendon plaatje  -->
						<img src="images/logo.png" width="280" height="82" alt=""/>
	
						<?php
							if (isset($_SESSION['errors'])): 
						?>	<!-- class toggler en id effect hoort bij jquery script, zie: index.js  -->
							<!-- Deze class en is zorgen samen voor het 'animatie' effect -->
							<div class="toggler">
							  <div style="max-width:100%;width:275px;" id="effect">
								<?php foreach($_SESSION['errors'] as $error): ?>
									<!-- class="ui-widget-header ui-corner-all" is een externe css  -->
									<!-- dus om de size van width naar ons gewenste instellingen 
									     aan te passen doen we het hier  -->
									<h3 style="max-width:100%;width:260px;" class="ui-widget-header ui-corner-all">
										<?php echo $error ?>
									</h3>
								<?php endforeach; ?>
							  </div>
							</div>
						<?php 
							session_destroy();
							endif; 
						?>		

						// PHP functie om de ZWave-connectie te starten
						// Door dev=/dev/ttyUSB0&fn=open&usb=false naar de ZWave-controlpanel te sturen
						// maakt de controlpanel verbinding met de ZStick
						// als de Pi niet aanstaat of verbonden is met VPN, quote deze functie dan
						// anders laadt de site niet
						<?php
/*							//in de array $postData2 zetten we de onderdelen van de postrequest
							//curl bouwt hier zelf een kloppende request van
 							$postData2 = array();
							$postData2['dev'] = '/dev/ttyUSB0';
							$postData2['fn'] = 'open';
							$postData2['usb'] = 'false';

							//devConn = handler voor de curlfunctie die we gaan bouwen
							//met curl_setopt voeg je opties toe
							$devConn = curl_init();
							curl_setopt($devConn, CURLOPT_URL, 'http://10.1.1.2:5555/devpost.html'); //form waar we naartoe gaan posten
							curl_setopt($devConn, CURLOPT_POST, 29); //lengte van het postrequest
							curl_setopt($devConn, CURLOPT_POSTFIELDS, $postData2); //velden die we gaan posten en inhoud daarvan
							curl_setopt($devConn, CURLOPT_RETURNTRANSFER, 1);

							//execute de opgebouwde curlstatement
							$output = curl_exec($devConn);
							curl_close($devConn); */
						?>
						<form action="php/login.php" method="POST">
							<input class="Login_Textbox" type="text" name="username" placeholder="Username" /><br />
							<input class="Login_Textbox" type="password" name="password" placeholder="Password" /><br /><br>
							<input class="Login_Button" type="submit" value="Login"><br><br>
						</form>
					</center>
				</div>
			</div>
		</div>
    </body>
</html>
