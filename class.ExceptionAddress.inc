<?php

class ExceptionAddress extends Exception {
    //overide __toStirng
    public function __toString() {
       return __CLASS__ . ": [{$this->code}]: {$this->message}\n ";
      
    }
}

