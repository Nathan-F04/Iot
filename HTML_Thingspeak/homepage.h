// To create literal strings stored in flash memory enclose your HTML code between 
// F(R"=====( HTML code here )=====");
// If you have 1 reading then you probably have 2 literal strings
// If you have 2 readings then you probably have 3 literal strings etc.

String homePagePartDHT11 = F(R"=====(<!doctype html>

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>NaviCar</title>
  <link rel="icon" type="image/x-icon" href="sample-logo.jpg">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.1/css/all.min.css" integrity="sha512-DTOQO9RWCH3ppGqcWaEA1BIZOC6xxalwEsw9c2QQeAIftl+Vegovlnee1c9QX4TctnWMn13TZye+giMm8e2LwA==" crossorigin="anonymous" referrerpolicy="no-referrer" />
  <style>
    .flex-Container{
        display: flex;
        flex-direction: column;
    }
    .flex-row {
        display: flex;
        flex-direction: column;
        align-items: center;
    }

    .flex-col{
	    display: flex;
        flex-direction: row;
	    align-items: center;
	    justify-content:space-evenly;
        border-bottom: 5px solid black;
        padding: 20px;
    }
    img{
        width: 200px;
        height: auto;
    }
    ul {
      float:left;
      list-style-type: none;
      margin: 0;
      padding: 0;
      overflow: hidden;
    }
     /*Navbar Css floating text/categories to left setting text color,background color,padding,width etc */
     .navbar li{
      float:left;
    }
    .navbar{
        background-color:dodgerblue;
    }
    .navbar a{
      color:black; /*color of text on navbar*/
      text-decoration:none;
      padding: 15px;
      display: block;
      text-align: center;
    }
    .navbar a:hover{
      background-color: blue;
    }
    html{
      scroll-behavior: smooth;
    }
    body{
        margin: 0px;
        padding: 0px;
    }
    #DHT11{
        background-color:aliceblue;

    }
    #GPS{
        background-color: blanchedalmond;
    }
    #Ultrasonic{
        background-color: darkgray;
    }
    #Motors{
        background-color: paleturquoise;
    }
  </style>

<script>
    // Get the button
    let mybutton = document.getElementById("myBtn");

    // When the user scrolls down 20px from the top of the document, show the button
    window.onscroll = function() {scrollFunction()};

    function scrollFunction() {
      if (document.body.scrollTop > 20 || document.documentElement.scrollTop > 20) {
        mybutton.style.display = "block";
      } else {
        mybutton.style.display = "none";
      }
    }

    // When the user clicks on the button, scroll to the top of the document
    function topFunction() {
      document.body.scrollTop = 0;
      document.documentElement.scrollTop = 0;
    }
  </script>

</head>

<body>
    <div class="flex-Container" >
        <nav class="navbar">
            <ul>
              <li><a href="#DHT11">DHT11</a></li>
              <li><a href="#GPS">GPS</a></li>
              <li><a href="#Ultrasonic">Ultrasonic</a></li>
              <li><a href="#Motors">DC Motors</a></li>
            </ul>
          </nav>
        <div class="flex-col" id="DHT11">
            <div class="flex-row">
                <h1>DHT11</h1>
                <p>Temp here)=====");
String homePagePartDHT112=F(R"===(</p>
                <p>Humidity here)===");

String homePagePartGPS = F(R"=====(</p>
            </div> 
            <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/2414971/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=10&type=line"></iframe>
            <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/2414971/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=5&type=line"></iframe>
        </div>
        <div class="flex-col" id="GPS">
            <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/2414971/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=10&type=line"></iframe>
            <div class="flex-row">
                <h1>GPS</h1>
                <p>Location here)=====");

String homePagePartLink = F(R"=====(<p>OPEN MAPS</p>)=====");

String homePagePartComponents = F(R"=====(</p>
            </div> 
        </div>
        <div class="flex-col" id="Ultrasonic">
            <div class="flex-row">
                <h1>Ultrasonic</h1>
                <p>Info on component</p>
            </div> 
            <img alt="n/a" src="Careers.jpg">
        </div>
        <div class="flex-col" id="Motors">
            <img alt="n/a" src="Careers.jpg">
            <div class="flex-row">
                <h1>DC Motors</h1>
                <p>Info on components</p>
            </div> 
        </div>
        <button onclick="topFunction()" id="myBtn" title="Go to top">Go to top</button>
    </div>

</body>)=====");