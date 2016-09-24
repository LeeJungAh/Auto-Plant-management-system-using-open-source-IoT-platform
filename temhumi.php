<?php
  mysql_connect("localhost", "root", "Ekfrl13") or die (mysql_error());

  mysql_select_db("strawberry");
  $q = $_REQUEST["q"];
  
 $sql="SELECT date_format((TEM_DT), '%H:%i:%s') as TEM_DT,temp,humi FROM temphumi WHERE date_format((TEM_DT), '%Y%m%d')=date_format('".$q."', '%Y%m%d')";
 
 $result = mysql_query( $sql ) or die (mysql_error());

$table = array();
$table['cols'] = array(
        array('label' => 'TEM_DT', 'type' => 'string'),
        array('label' => 'temp', 'type' => 'number'),
        array('label' => 'humi', 'type' => 'number')
);
$rows = array();

    while($row = mysql_fetch_array($result))

    {

        $tem = array();
        // each column needs to have data inserted via the $temp array
        $tem[] = array('v' => $row['TEM_DT']);
        $tem[] = array('v' => (int) $row['temp']);
        $tem[] = array('v' => (int) $row['humi']); 

        // insert the temp array into $rows
        $rows[] = array('c' => $tem);
    }
 $table['rows'] = $rows;

        // encode the table as JSON
        $jsonTable = json_encode($table);
        echo $jsonTable;

?>

