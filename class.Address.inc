<?php

abstract class Address implements Model {
    
    const ADDRESS_TYPE_RESIDENCE = 1;
    const ADDRESS_TYPE_BUSINESS = 2;
    const ADDRESS_TYPE_PARK = 3;
    
    const ADDRESS_ERROR_NOT_FOUND = 1000;
    
    //address types
    static public $valid_address_types = array (
        ADDRESS::ADDRESS_TYPE_RESIDENCE => 'Residence',
        ADDRESS::ADDRESS_TYPE_BUSINESS => 'Business',
        ADDRESS::ADDRESS_TYPE_PARK => 'Park'
        );
    
      
        //sTREET adrees
    
    public $street_address_1;
    public $street_address_2;
    
    //name of city
    public $city_name;
    
    //name of subdivision
    public $subdivision_name;
    
    //postal code
    protected $_postal_code;
    
    //name of country
    public $country_name;
    
    //primary key of address
    protected $_address_id;
    
    protected $_address_type_id;
    
    //when the record was creates and last updates
    protected $_time_created;
    protected $_time_updated;
    
    /**
     * post clone behavior
     */
    function __clone() {
        
    }
/**
   * Constructor.
   * @param array $data Optional array of property names and values.
   */
  function __construct($data = array()) {
     $this->_init();
     $this->_time_created = time();
    
    // Ensure that the Address can be populated.
    if (!is_array($data)) {
      trigger_error('Unable to construct address with a ' . get_class($name));
    }
    
    // If there is at least one value, populate the Address with it.
    if (count($data) > 0) {
      foreach ($data as $name => $value) {
        // Special case for protected properties.
        if (in_array($name, array(
          'time_created',
          'time_updated',
            'address_id',
            'address_type_id',
        ))) {
          $name = '_' . $name;
        }
        $this->$name = $value;
      }
    }
  }
    
    /**
     * Magic __get.
     * @param string $name
     * @return mixed
     * 
     */
    function __get($name) {
       //potal code lookup if unset
        if (!$this->_postal_code) {
            $this->_postal_code = $this->_postal_code_guess();
        }
        
        //attempt to return prtoected propery
        $protected_property_name ='_'.$name;
        if (property_exists($this, $protected_property_name)){
            return $this->$protected_property_name;
        }
        
        //Unable to access property; trigger error
        trigger_error('undefined property via __get: '.$name);
        return NULL; 
    }
    
    /**
     * 
     * @param string$name
     * @param mixxed $value
     */
    function __set($name, $value) {
            // Only set valid address type id.
        //allow anything to set postal code
        if ('postal_code' ==$name) {
            $this->$name = $value;
            return;
        }
        //unable toa ccess property
        trigger_error('undiefined property via __set(): '.$name);
    }
    
    function __toString() {
        return $this->display();
    }
    
    /**
     * force extending class to implement init
     */
    abstract protected function _init();
        
   

    /**
     * guess the postal code based on info
     * 
     * 
     */
  protected function _postal_code_guess() {
    $db = Database::getInstance();
    $mysqli = $db->getConnection();
    
    $sql_query  = 'SELECT postal_code ';
    $sql_query .= 'FROM location ';
    
    $city_name = $mysqli->real_escape_string($this->city_name);
    $sql_query .= 'WHERE city_name = "' . $city_name . '" ';
    
    $subdivision_name = $mysqli->real_escape_string($this->subdivision_name);
    $sql_query .= 'AND subdivision_name = "' . $subdivision_name . '" ';
    
    $result = $mysqli->query($sql_query);
    
    if ($row = $result->fetch_assoc()) {
      return $row['postal_code'];
    }
  }


    /**
     * Display an address in html
     * @return string
     */
    function display() {
        $output = '';
                
        //street address
        $output .= $this->street_address_1;
        if ($this->street_address_2) {
            $output .= '<br/>' .$this->street_address_2;
        }
        
        //city, subdivision, postal
        
        $output .='<br/>';
        $output .= $this->city_name . ', ' . $this->subdivision_name;
        $output .= ' ' .$this->postal_code;
            
        //sountry
        $output .= '<br/>';
        $output .= $this->country_name;
        
        return $output;   
        }
        
        /**
         * 
         * @param int $address_type_id
         * @return boolean
         * 
         */
        static public function  isValidAddressTypeId($address_type_id) {
            return array_key_exists($address_type_id, self::$valid_address_types);
        }
        
         /**
        * If valid, set the address type id.
        * @param int $address_type_id 
        */
       protected function _setAddressTypeId($address_type_id) {
         if (self::isValidAddressTypeId($address_type_id)) {
           $this->_address_type_id = $address_type_id;
    }
  }  
  
  final public static function load($address_id){
      $db = Database::getInstance();
      $mysqli = $db->getConnection();
      
          $sql_query  = 'SELECT address.* ';
    $sql_query .= 'FROM address ';
    $sql_query .= 'WHERE address_id = "' . (int) $address_id . '" ';
    
    $result = $mysqli->query($sql_query);
    if ($row = $result->fetch_assoc()) {
      return self::getInstance($row['address_type_id'], $row);
    }
    throw new ExceptionAddress('address not found', self::ADDRESS_ERROR_NOT_FOUND);
  }
  
  /**
   * Given an address_type_id, return an instance of that subclass.
   * @param int $address_type_id
   * @param array $data 
   * @return Address subclass
   */
  final public static function getInstance($address_type_id, $data = array()) {
    $class_name = 'Address' . self::$valid_address_types[$address_type_id];
    return new $class_name($data);
  }
  
  final public function save() {}
}