<?php

/**
 * @file
 * Page with LaTeX generate admin form
 */

function latex_generate_admin_form() {
    $form = array();

    $form['years'] = array(
        '#title' => t('Academic year'),
        '#type' => 'textfield',
        '#required' => TRUE,
        'size' => 10,
    );
    $form['date_assigned'] = array(
        '#title' => t('Date assigned'),
        '#type' => 'date_popup',
        '#default_value' => date('d. m. Y'),
        '#date_type' => DATE_DATETIME,
        '#date_timezone' => date_default_timezone(),
        '#date_format' => 'd. m. Y',
        '#date_increment' => 1,
        '#date_year_range' => '-5:+3',
    );
    $form['date_due'] = array(
        '#title' => t('Date assigned'),
        '#type' => 'date_popup',
        '#default_value' => date('d. m. Y'),
        '#date_type' => DATE_DATETIME,
        '#date_timezone' => date_default_timezone(),
        '#date_format' => 'd. m. Y',
        '#date_increment' => 1,
        '#date_year_range' => '-5:+3',
    );


    return system_settings_form($form);
}