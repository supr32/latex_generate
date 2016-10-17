<?php

/**
 * @file
 * Page with LaTeX generate form.
 */

/**
 * Page.
 */
function latex_generate_input_form() {
  $form = array();

  $form['for'] = array(
    // @ToDo This title is not helpful.
    '#title' => t('For'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 60,
  );
  $form['title_cz'] = array(
    '#title' => t('Title in czech'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 60,
  );
  $form['title_en'] = array(
    '#title' => t('Title in english'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 60,
  );
  $form['outline'] = array(
    '#title' => t('Outline'),
    '#type' => 'textarea',
    '#required' => TRUE,
  );
  $form['literature'] = array(
    '#title' => t('Literature'),
    '#description' => t(
      'Use !link - you can search books by ISBN',
      array(
        '!link' => l(
          'generator.citace.com',
          'http://generator.citace.com'
        ),
      )
    ),
    '#type' => 'textarea',
    '#required' => TRUE,
  );
  $form['supervisor'] = array(
    '#title' => t('Thesis supervisor'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 60,
  );
  $form['superaddress'] = array(
    '#title' => t('Thesis supervisor address'),
    '#type' => 'textarea',
    '#required' => TRUE,
    );

  $form['textdate'] = array(
  '#markup' => '<p><label style="display:inline">Datum zadání</label>: 20.10.2015</p><p><label style="display:inline">Datum odevzdání</label>: 08.07.2016</p><script>
    var editor = new wysihtml5.Editor("edit-literature", {
        parserRules:  wysihtml5ParserRules 
    });
</script>'
  );

  // Set default values if any.
  if ($_SESSION['latex_generate_defaults']) {
    foreach ($form as $key => $element) {
      if (isset($form[$key]) && isset($_SESSION['latex_generate_defaults'][$key])) {
        $form[$key]['#default_value'] = $_SESSION['latex_generate_defaults'][$key];
      }
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
  );

  return $form;
}

/**
 * Validate callback for Page.
 */
function latex_generate_input_form_validate(&$form, &$form_state) {
  // Check if dates are sane.
  $date_assigned = strtotime($form_state['values']['date_assigned']);
  $date_due = strtotime($form_state['values']['date_due']);
  if ($date_assigned > $date_due) {
    form_set_error('date_due', t("Date due can't be earlier than Date assigned."));
  }
}

/**
 * Submit callback for Page.
 */
function latex_generate_input_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  unset($values['form_build_id']);
  unset($values['form_token']);
  unset($values['form_id']);
  unset($values['op']);
  unset($values['submit']);
  $_SESSION['latex_generate_defaults'] = $values;

  module_load_include('inc', 'latex_generate', 'includes/latex_generate.latex');

  // Get text data to be saved and transfered.
  $data = latex_generate_generate($values);

  // Generate file name.
  $filename = 'result_' . time();
  if (function_exists('transliteration_clean_filename')) {
    $filename = transliteration_clean_filename(check_plain($values['title_en']) . '_' . time());
  }

  // Try to generate PDF. (Falls back to .tex automatically).
  $file = latex_generate_input_handle_pdf($filename, $data);

  // Transfer the file to user.
  $headers = array(
    'Content-disposition' => 'attachment; filename=' . $file->filename,
  );
  file_transfer($file->uri, $headers);
}
