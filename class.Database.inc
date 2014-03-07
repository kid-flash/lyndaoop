<?php

/**
 * MYSQLi database, only allows one conenction
 */
class Database {
    private $_connection;
    //store the single instance
    private static $_instance;
    
    /**
     * Get instance of the Database
     * @return Database
     */
    public static function getInstance() {
        if (!self::$_instance) {
            self::$_instance = new self();
        }
        return self::$_instance;
    }
    
    /**
     * Constructor
     */
    public function __construct(){
        $this->_connection = new mysqli('localhost', 'root', '', 'sandbox');
               // Error handling.
    if (mysqli_connect_error()) {
      trigger_error('Failed to connect to MySQL: ' . mysqli_connect_error(), E_USER_ERROR);
    }
    }
    
    private function __clone(){}

    public function getConnection() {
        return $this->_connection;
    }
    
}
