<?php

/**
 * Park Address
 */
class AddressPark extends Address {
    public function display() {
        $output  = '<div style="background-color:palegreen;"';
        $output .= parent::display();
        $output .= '</div>';
        return $output;
    }
    
    protected function _init() {
        $this->_setAddressTypeId(Address::ADDRESS_TYPE_PARK);
    }
}

