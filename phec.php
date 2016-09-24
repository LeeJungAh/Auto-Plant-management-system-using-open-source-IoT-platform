<?php
  mysql_connect("localhost", "root", "Ekfrl13") or die (mysql_error());

  mysql_select_db("strawberry");
  $q = $_REQUEST["q"];
  
 $sql="SELECT date_format((phtime), '%H:%i:%s') as phtime,pH,ec FROM ph WHERE date_format((phtime), '%Y-%m-%d')=date_format('".$q."', '%Y-%m-%d')";

  $result = mysql_query( $sql ) or die (mysql_error());

$table = array();
$table['cols'] = array(
        array('label' => 'phtime', 'type' => 'string'),
        array('label' => 'pH', 'type' => 'number'),
        array('label' => 'ec', 'type' => 'number')
);
$rows = array();

    while($row = mysql_fetch_array($result))

    {

        $tem = array();
        // each column needs to have data inserted via the $temp array
        $tem[] = array('v' =>  $row['TEM_DT']);
        $tem[] = array('v' => (float) $row['pH']);
        $tem[] = array('v' => (float) $row['ec']);

        // insert the temp array into $rows
        $rows[] = array('c' => $tem);
    }
 $table['rows'] = $rows;

        // encode the table as JSON
        $jsonTable = json_encode($table);
        echo $jsonTable;

?>

