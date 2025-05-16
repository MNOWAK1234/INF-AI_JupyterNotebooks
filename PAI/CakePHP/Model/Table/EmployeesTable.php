<?php
declare(strict_types=1);

namespace App\Model\Table;

use Cake\ORM\Query;
use Cake\ORM\RulesChecker;
use Cake\ORM\Table;
use Cake\Validation\Validator;

/**
 * Employees Model
 *
 * @method \App\Model\Entity\Employee newEmptyEntity()
 * @method \App\Model\Entity\Employee newEntity(array $data, array $options = [])
 * @method \App\Model\Entity\Employee[] newEntities(array $data, array $options = [])
 * @method \App\Model\Entity\Employee get($primaryKey, $options = [])
 * @method \App\Model\Entity\Employee findOrCreate($search, ?callable $callback = null, $options = [])
 * @method \App\Model\Entity\Employee patchEntity(\Cake\Datasource\EntityInterface $entity, array $data, array $options = [])
 * @method \App\Model\Entity\Employee[] patchEntities(iterable $entities, array $data, array $options = [])
 * @method \App\Model\Entity\Employee|false save(\Cake\Datasource\EntityInterface $entity, $options = [])
 * @method \App\Model\Entity\Employee saveOrFail(\Cake\Datasource\EntityInterface $entity, $options = [])
 * @method \App\Model\Entity\Employee[]|\Cake\Datasource\ResultSetInterface|false saveMany(iterable $entities, $options = [])
 * @method \App\Model\Entity\Employee[]|\Cake\Datasource\ResultSetInterface saveManyOrFail(iterable $entities, $options = [])
 * @method \App\Model\Entity\Employee[]|\Cake\Datasource\ResultSetInterface|false deleteMany(iterable $entities, $options = [])
 * @method \App\Model\Entity\Employee[]|\Cake\Datasource\ResultSetInterface deleteManyOrFail(iterable $entities, $options = [])
 */
class EmployeesTable extends Table
{
    /**
     * Initialize method
     *
     * @param array $config The configuration for the Table.
     * @return void
     */
    public function initialize(array $config): void
    {
        parent::initialize($config);

        $this->setTable('employees');
        $this->setDisplayField('id');
        $this->setPrimaryKey('id');
    }

    /**
     * Default validation rules.
     *
     * @param \Cake\Validation\Validator $validator Validator instance.
     * @return \Cake\Validation\Validator
     */
    public function validationDefault(Validator $validator): Validator
    {
        $validator
            ->scalar('nazwisko')
            ->maxLength('nazwisko', 15)
            ->notEmptyString('nazwisko', 'Nazwisko jest wymagane');

        $validator
            ->scalar('imie')
            ->maxLength('imie', 15)
            ->allowEmptyString('imie');

        $validator
            ->requirePresence('etat')
            ->notEmptyString('etat', 'Etat jest wymagany')
            ->inList('etat', ['Pracownik', 'Manager', 'Dyrektor'], 'Nieprawidłowy etat');

        $validator
            ->decimal('id_szefa')
            ->allowEmptyString('id_szefa');

        $validator
            ->date('zatrudniony')
            ->allowEmptyDate('zatrudniony');

        $validator
            ->decimal('placa_pod')
            ->allowEmptyString('placa_pod')
            ->range('placa_pod', [0, 2000]);

        $validator
            ->decimal('placa_dod')
            ->allowEmptyString('placa_dod');

        $validator
            ->decimal('id_zesp')
            ->allowEmptyString('id_zesp');

        return $validator;
    }
}
