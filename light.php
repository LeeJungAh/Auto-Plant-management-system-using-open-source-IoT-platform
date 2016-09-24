<?php
  mysql_connect("localhost", "root", "Ekfrl13") or die (mysql_error());

  mysql_select_db("strawberry");
  $q = $_REQUEST["q"];
  
 $sql="SELECT date_format((light_time), '%H:%i:%s') as light_time,lightValue FROM light WHERE date_format((light_time), '%Y-%m-%d')=date_format('".$q."', '%Y-%m-%d')";

  $result = mysql_query( $sql ) or die (mysql_error());

$table = array();
$table['cols'] = array(
        array('label' => 'light_time', 'type' => 'string'),
        array('label' => 'lightValue', 'type' => 'number'),
);
$rows = array();

    while($row = mysql_fetch_array($result))

    {

        $tem = array();
        // each column needs to have data inserted via the $temp array
        $tem[] = array('v' =>  $row['light_time']);
        $tem[] = array('v' => (float) $row['lightValue']);

        // insert the temp array into $rows
        $rows[] = array('c' => $tem);
    }
 $table['rows'] = $rows;

        // encode the table as JSON
        $jsonTable = json_encode($table);
        echo $jsonTable;

?>

