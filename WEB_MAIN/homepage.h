// To create literal strings stored in flash memory enclose your HTML code between 
// F(R"=====( HTML code here )=====");
// If you have 1 reading then you probably have 2 literal strings
// If you have 2 readings then you probably have 3 literal strings etc.

String homePagePart1 = F(R"=====(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="refresh" content="5">
    <title>NaviCar</title>
    <link rel="icon" type="image/x-icon" href="TestLogo1.png">
    <style>
      body{
          margin: 0px;
          background-image: url(Testlogo1.png);
          background-repeat: no-repeat;
          background-attachment: fixed;
          background-position: center;
          background-size: 100% 100%;

    }
    main{
        margin-left:20px;
        margin-right: 20px;
    }   
    ul {/*Had this as .navbar but didn't put background color full from left to right on screen*/
        list-style-type: none;
        margin: 0;
        padding: 0;
        overflow: hidden;
        background-color: #1b2399;/*color of background on navbar*/
        border: 3px solid #e7e7e7;/*Border width and color*/
    /*position: fixed; These three lines put navbar at top even when scrolling but my header dissapears under it need to research further
    top: 0;
    width: 100%;*/
    }

    .navbar a{
        color:rgb(255, 255, 255); /*color of text on navbar*/
        text-decoration:none;
        padding: 15px;
        display: block;
        text-align: center;
    }
    .navbar a:hover{
        background-color: black;
    }
    li{
        float:left;
    }
  </style>


</head>
<body>



    <nav class="navbar">
        <ul>
            <li><a href="#GPS">GPS</a></li>
            <li><a href="#Motor">Motor Status</a></li>
            <li><a href="#Ultrasonic">Ultrasonic Status</a></li>
            <li><a href="#SEN0017">SEN0017 Status</a></li>
            <li><a href="#DHT11">DHT11 Status</a></li>
            <li><a href="#Servo">Servo Status</a></li>
        </ul>
    </nav>
    

    <main>
        <div id="GPS">
            <h3>GPS info:</h3>
            <table>
                <tr>
                  <th>GPS Location:</th>
                  <th>)=====");
String homePagePart2 = F(R"=====(</th>
                </tr>
         </table>
        </div>

        <div id="Motor">
            <h3> Motor info:</h3>
            <table>
                <tr>
                  <th>Motor Mode:</th>
                  <th>)=====");
String homePagePart3 = F(R"=====(</th>
                </tr>
         </table>
        </div>

        <div id="Ultrasonic">
            <h3>Ultrasonic info:</h3>
            <table>
                <tr>
                  <th>Distance:</th>
                  <th>)=====");
String homePagePart4 = F(R"=====(</th>
                </tr>
         </table>
        </div>

        <div id="SEN0017">
            <h3>SEN0017 info:</h3>
            <table>
                <tr>
                  <th>Reading:</th>
                  <th>)=====");
String homePagePart5 = F(R"=====(</th>
                </tr>
         </table>
        </div>


        <div id="DHT11">
            <h3>DHT11 info:</h3>
            <table>
                <tr>
                  <th>Temperature:</th>
                  <th>)=====");
String homePagePart6 = F(R"=====(</th>
                </tr>
         </table>
            
        </div>

        <div id="Servo">
            <h3>Servo info:</h3>
            <table>
                <tr>
                  <th>Mode:</th>
                  <th>)=====");
String homePagePart7 = F(R"=====(</th>
                </tr>
         </table>
        </div>
    </main>

</body>
</html>)=====");