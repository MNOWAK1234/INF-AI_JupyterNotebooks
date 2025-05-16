<?php
namespace App\Model\Entity;

use Cake\ORM\Entity;

class Book extends Entity
{
    protected $_accessible = [
        '*' => false,
        'id' => false,
        'title' => true,
        'author' => true,
        'genre' => true
    ];
}
