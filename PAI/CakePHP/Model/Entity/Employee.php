<?php
declare(strict_types=1);

namespace App\Model\Entity;

use Cake\ORM\Entity;

/**
 * Employee Entity
 *
 * @property int $id
 * @property string|null $nazwisko
 * @property string|null $imie
 * @property string|null $etat
 * @property string|null $id_szefa
 * @property \Cake\I18n\FrozenDate|null $zatrudniony
 * @property string|null $placa_pod
 * @property string|null $placa_dod
 * @property string|null $id_zesp
 */
class Employee extends Entity
{
    /**
     * Fields that can be mass assigned using newEntity() or patchEntity().
     *
     * Note that when '*' is set to true, this allows all unspecified fields to
     * be mass assigned. For security purposes, it is advised to set '*' to false
     * (or remove it), and explicitly make individual fields accessible as needed.
     *
     * @var array<string, bool>
     */
    protected $_accessible = [
        'nazwisko' => true,
        'imie' => true,
        'etat' => true,
        'id_szefa' => true,
        'zatrudniony' => true,
        'placa_pod' => true,
        'placa_dod' => true,
        'id_zesp' => true,
    ];
}
