<?php
namespace App\Model\Table;

use Cake\ORM\Table;
use Cake\Validation\Validator;

class BooksTable extends Table
{
    function validationDefault(Validator $validator): Validator
    {
        $validator
            ->notEmptyString('title')
            ->minLength('title', 4)
            ->maxLength('title', 255);
        return $validator;
    }

}
