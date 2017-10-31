<?php
// $Id$
function circuit_simulation_abstract()
{
	global $user;
	$return_html = "";
	$proposal_data = circuit_simulation_get_proposal();
	if (!$proposal_data)
	{
		drupal_goto('');
		return;
	} //!$proposal_data
	//$return_html .= l('Upload abstract', 'circuit-simulation-project/abstract-code/upload') . '<br />';
	/* get experiment list */
	$query = db_select('esim_circuit_simulation_submitted_abstracts');
	$query->fields('esim_circuit_simulation_submitted_abstracts');
	$query->condition('proposal_id', $proposal_data->id);
	$abstracts_q = $query->execute()->fetchObject();
	if ($abstracts_q)
	{
		if ($abstracts_q->is_submitted == 1)
		{
			drupal_set_message(t('Your abstract is under review, you can not edit exisiting abstract without reviewer permission.'), 'error', $repeat = FALSE);
			//drupal_goto('circuit-simulation-project/abstract-code');
			//return;
		} //$abstracts_q->is_submitted == 1
	} //$abstracts_q
	$query_pro = db_select('esim_circuit_simulation_proposal');
	$query_pro->fields('esim_circuit_simulation_proposal');
	$query_pro->condition('id', $proposal_data->id);
	$abstracts_pro = $query_pro->execute()->fetchObject();
	$query_pdf = db_select('esim_circuit_simulation_submitted_abstracts_file');
	$query_pdf->fields('esim_circuit_simulation_submitted_abstracts_file');
	$query_pdf->condition('proposal_id', $proposal_data->id);
	$query_pdf->condition('filetype', 'A');
	$abstracts_pdf = $query_pdf->execute()->fetchObject();
	if ($abstracts_pdf == TRUE)
	{
		if ($abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != "")
		{
			$abstract_filename = $abstracts_pdf->filename;
		} //$abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != ""
		else
		{
			$abstract_filename = "File not uploaded";
		}
	} //$abstracts_pdf == TRUE
	else
	{
		$abstract_filename = "File not uploaded";
	}
	$query_process = db_select('esim_circuit_simulation_submitted_abstracts_file');
	$query_process->fields('esim_circuit_simulation_submitted_abstracts_file');
	$query_process->condition('proposal_id', $proposal_data->id);
	$query_process->condition('filetype', 'S');
	$abstracts_query_process = $query_process->execute()->fetchObject();
	if ($abstracts_query_process == TRUE)
	{
		if ($abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != "")
		{
			$abstracts_query_process_filename = $abstracts_query_process->filename;
		} //$abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != ""
		else
		{
			$abstracts_query_process_filename = "File not uploaded";
		}
		if ($abstracts_q->is_submitted == '')
		{
			$url = l('Upload abstract', 'circuit-simulation-project/abstract-code/upload');
		} //$abstracts_q->is_submitted == ''
		else if ($abstracts_q->is_submitted == 1)
		{
			$url = "";
		} //$abstracts_q->is_submitted == 1
		else if ($abstracts_q->is_submitted == 0)
		{
			$url = l('Edit abstract', 'circuit-simulation-project/abstract-code/upload');
		} //$abstracts_q->is_submitted == 0
		if ($abstracts_q->unit_operations_used_in_esim == '')
		{
			$unit_operations_used_in_esim = "Not entered";
		} //$abstracts_q->unit_operations_used_in_esim == ''
		else
		{
			$unit_operations_used_in_esim = $abstracts_q->unit_operations_used_in_esim;
		}
		if ($abstracts_q->thermodynamic_packages_used == '')
		{
			$thermodynamic_packages_used = "Not entered";
		} //$abstracts_q->thermodynamic_packages_used == ''
		else
		{
			$thermodynamic_packages_used = $abstracts_q->thermodynamic_packages_used;
		}
		if ($abstracts_q->logical_blocks_used == '')
		{
			$logical_blocks_used = "Not entered";
		} //$abstracts_q->logical_blocks_used == ''
		else
		{
			$logical_blocks_used = $abstracts_q->logical_blocks_used;
		}
	} //$abstracts_query_process == TRUE
	else
	{
		$url = l('Upload abstract', 'circuit-simulation-project/abstract-code/upload');
		$unit_operations_used_in_esim = "Not entered";
		$thermodynamic_packages_used = "Not entered";
		$logical_blocks_used = "Not entered";
		$abstracts_query_process_filename = "File not uploaded";
	}
	$headers = array(
		"Name of compound for which process development is carried out",
		"CAS No."
	);
	$rows = array();
	$item = array(
		"{$proposal_data->process_development_compound_name}",
		"{$proposal_data->process_development_compound_cas_number}"
	);
	array_push($rows, $item);
	$prodata = theme('table', array(
		'header' => $headers,
		'rows' => $rows
	));
	$uploaded_user_defined_compound_filepath = basename($proposal_data->user_defined_compound_filepath) ? basename($proposal_data->user_defined_compound_filepath) : "Not uploaded";
	$return_html .= '<strong>Proposer Name:</strong><br />' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '<br /><br />';
	$return_html .= '<strong>Title of the Flowsheet Project:</strong><br />' . $proposal_data->project_title . '<br /><br />';
	$return_html .= '<strong>eSim version:</strong><br />' . $proposal_data->version . '<br /><br />';
	$return_html .= '<strong>Unit Operations used in eSim:</strong><br />' . $unit_operations_used_in_esim . '<br /><br />';
	$return_html .= '<strong>Thermodynamic Packages Used:</strong><br />' . $thermodynamic_packages_used . '<br /><br />';
	$return_html .= '<strong>Logical Blocks used:</strong><br />' . $logical_blocks_used . '<br /><br />';
	$return_html .= '<strong>Name of compound for which process development is carried out:</strong><br />' . $prodata . '<br />';
	$return_html .= '<strong>List of compounds from eSim Database used in process circuit simulation:</strong><br />' . $proposal_data->esim_database_compound_name . '<br /><br />';
	$return_html .= '<strong>List of user defined compounds used in process circuit simulation:</strong><br />' . _circuit_simulation_list_of_user_defined_compound($proposal_data->id) . '<br />';
	$return_html .= '<strong>Uploaded user defined compound file:</strong><br />' . $uploaded_user_defined_compound_filepath . '<br /><br />';
	$return_html .= '<strong>Uploaded an abstract (brief outline) of the project:</strong><br />' . $abstract_filename . '<br /><br />';
	$return_html .= '<strong>Upload the eSim circuit simulation for the developed process:</strong><br />' . $abstracts_query_process_filename . '<br /><br />';
	$return_html .= $url . '<br />';
	return $return_html;
}
function circuit_simulation_upload_abstract_code_form($form, &$form_state)
{
	global $user;
	$form['#attributes'] = array(
		'enctype' => "multipart/form-data"
	);
	/* get current proposal */
	//$proposal_id = (int) arg(3);
	$uid = $user->uid;
	//$proposal_q = db_query("SELECT * FROM {circuit_simulation_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('circuit_simulation_proposal');
	$query->fields('circuit_simulation_proposal');
	$query->condition('uid', $uid);
	$query->condition('approval_status', '1');
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
			drupal_goto('circuit-simulation-project/abstract-code');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('circuit-simulation-project/abstract-code');
		return;
	}
	$query = db_select('circuit_simulation_submitted_abstracts');
	$query->fields('circuit_simulation_submitted_abstracts');
	$query->condition('proposal_id', $proposal_data->id);
	$abstracts_q = $query->execute()->fetchObject();
	if ($abstracts_q)
	{
		if ($abstracts_q->is_submitted == 1)
		{
			drupal_set_message(t('Your abstract is under review, you can not edit exisiting abstract without reviewer permission.'), 'error', $repeat = FALSE);
			drupal_goto('circuit-simulation-project/abstract-code');
			//return;
		} //$abstracts_q->is_submitted == 1
	} //$abstracts_q->is_submitted == 1
	$form['project_title'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->project_title,
		'#title' => t('Title of the Flowsheet Project')
	);
	$form['version'] = array(
		'#type' => 'item',
		'#title' => t('eSim version'),
		'#markup' => $proposal_data->version
	);
	if ($abstracts_q == TRUE)
	{
		if ($abstracts_q->unit_operations_used_in_esim)
		{
			$existing_unit_operations_used_in_esim = default_value_for_selections("unit_operations_used_in_esim", $proposal_data->id);
			$form['unit_operations_used_in_esim'] = array(
				'#type' => 'select',
				'#title' => t('Unit Operations used in eSim'),
				'#options' => _df_list_of_unit_operations(),
				'#required' => TRUE,
				'#default_value' => $existing_unit_operations_used_in_esim,
				'#size' => '20',
				'#multiple' => TRUE,
				'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
			);
		} //$abstracts_q->unit_operations_used_in_esim
	} //$abstracts_q->unit_operations_used_in_esim
	else
	{
		$form['unit_operations_used_in_esim'] = array(
			'#type' => 'select',
			'#title' => t('Unit Operations used in eSim'),
			'#options' => _df_list_of_unit_operations(),
			'#required' => TRUE,
			'#size' => '20',
			'#multiple' => TRUE,
			'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
		);
	}
	if ($abstracts_q == TRUE)
	{
		if ($abstracts_q->thermodynamic_packages_used)
		{
			$existing_thermodynamic_packages_used = default_value_for_selections("thermodynamic_packages_used", $proposal_data->id);
			$form['thermodynamic_packages_used'] = array(
				'#type' => 'select',
				'#title' => t('Thermodynamic Packages Used'),
				'#options' => _df_list_of_thermodynamic_packages(),
				'#required' => TRUE,
				'#size' => '20',
				'#default_value' => $existing_thermodynamic_packages_used,
				'#multiple' => TRUE,
				'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
			);
		} //$abstracts_q->thermodynamic_packages_used
	} //$abstracts_q == TRUE
	else
	{
		$form['thermodynamic_packages_used'] = array(
			'#type' => 'select',
			'#title' => t('Thermodynamic Packages Used'),
			'#options' => _df_list_of_thermodynamic_packages(),
			'#required' => TRUE,
			'#size' => '20',
			'#multiple' => TRUE,
			'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
		);
	}
	if ($abstracts_q == TRUE)
	{
		//var_dump($abstracts_q->logical_blocks_used);die;
		if ($abstracts_q->logical_blocks_used != "Not entered")
		{
			$existing_logical_blocks_used = default_value_for_selections("logical_blocks_used", $proposal_data->id);
			$form['logical_blocks_used'] = array(
				'#type' => 'select',
				'#title' => t('Logical Blocks used (If any)'),
				'#options' => _df_list_of_logical_block(),
				'#default_value' => $existing_logical_blocks_used,
				'#multiple' => TRUE,
				'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
			);
		} //$abstracts_q->logical_blocks_used != "Not entered"
		else
		{
			$form['logical_blocks_used'] = array(
				'#type' => 'select',
				'#title' => t('Logical Blocks used (If any)'),
				'#options' => _df_list_of_logical_block(),
				'#multiple' => TRUE,
				'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
			);
		}
	} //$abstracts_q == TRUE
	else
	{
		$form['logical_blocks_used'] = array(
			'#type' => 'select',
			'#title' => t('Logical Blocks used (If any)'),
			'#options' => _df_list_of_logical_block(),
			'#multiple' => TRUE,
			'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
		);
	}
	$headers = array(
		"Name of compound for which process development is carried out",
		"CAS No."
	);
	$rows = array();
	$item = array(
		"{$proposal_data->process_development_compound_name}",
		"{$proposal_data->process_development_compound_cas_number}"
	);
	array_push($rows, $item);
	$prodata = theme('table', array(
		'header' => $headers,
		'rows' => $rows
	));
	$form['process_development_compound_name'] = array(
		'#type' => 'item',
		'#title' => t('Name of compound for which process development is carried out'),
		'#markup' => $prodata
	);
	if ($proposal_data->esim_database_compound_name)
	{
		$existing_esim_database_compound_name = default_value_for_selections("esim_database_compound_name", $proposal_data->id);
		$form['list_of_compounds_from_esim_database_used_in_process_circuit_simulation'] = array(
			'#type' => 'select',
			'#title' => t('List of compounds from eSim Database used in process circuit simulation'),
			'#options' => _df_list_of_esim_compound(),
			'#default_value' => $existing_esim_database_compound_name,
			'#size' => '20',
			'#multiple' => TRUE,
			'#description' => t('[You can select multiple options by holding ctrl + left key of mouse]')
		);
	} //$proposal_data->esim_database_compound_name
	else
	{
		$form['list_of_compounds_from_esim_database_used_in_process_circuit_simulation'] = array(
			'#type' => 'slect',
			'#title' => t('List of compounds from eSim Database used in process circuit simulation'),
			'#options' => _df_list_of_esim_compound(),
			'#size' => '20',
			'#multiple' => TRUE
		);
	}
	/////////////////////////////////////////////////////
	//Edit user defiend compounds
	$query_u = db_select('circuit_simulation_user_defined_compound');
	$query_u->fields('circuit_simulation_user_defined_compound');
	$query_u->condition('proposal_id', $proposal_data->id);
	$result_u = $query_u->execute();
	$num_of_user_defined_compounds_results = $result_u->rowCount();
	$form['user_defined_compound_fieldset'] = array(
		'#type' => 'fieldset',
		'#tree' => TRUE,
		'#prefix' => '<div id="user-defined-compounds-fieldset-wrapper">',
		'#suffix' => '</div>'
	);
	if ($num_of_user_defined_compounds_results != 0)
	{
		$form_state['num_user_defined_compounds'] = $num_of_user_defined_compounds_results;
		$temp = 0;
		$i = 0;
		while ($row_udc = $result_u->fetchObject())
		{
			$temp = $i;
			$form['user_defined_compound_fieldset'][$i]["s_text"] = array(
				"#type" => "item",
				"#markup" => "<h4><label>User defined compounds : " . ($temp + 1) . "</label></h4>"
			);
			$form['user_defined_compound_fieldset'][$i]["udc_id"] = array(
				"#type" => "hidden",
				"#default_value" => $row_udc->id
			);
			$form['user_defined_compound_fieldset'][$i]["user_defined_compound"] = array(
				"#type" => "textfield",
				"#title" => "Name of the user defined compound",
				"#default_value" => $row_udc->user_defined_compound
			);
			$form['user_defined_compound_fieldset'][$i]["cas_no"] = array(
				"#type" => "textfield",
				"#title" => "CAS No.",
				"#default_value" => $row_udc->cas_no
			);
			$i++;
		} //$row_udc = $result_u->fetchObject()
		$form['user_defined_compound_fieldset']["user_defined_compound_count"] = array(
			"#type" => "hidden",
			"#value" => $temp
		);
		/*$form['user_defined_compound_fieldset']['add_user_defined_compounds'] = array(
			'#type' => 'submit',
			'#value' => t('Add more compounds'),
			'#limit_validation_errors' => array(),
			'#submit' => array(
				'user_defined_compounds_add_more_add_one'
			),
			'#ajax' => array(
				'callback' => 'user_defined_compounds_add_more_callback',
				'wrapper' => 'user-defined-compounds-fieldset-wrapper'
			)
		);*/
		////////////////////////////
		$existing_uploaded_udc_file = default_value_for_uploaded_files("UDC", $proposal_data->id);
		if (!$existing_uploaded_udc_file)
		{
			$existing_uploaded_udc_file = new stdClass();
			$existing_uploaded_udc_file->filename = "No file uploaded";
		} //!$existing_uploaded_udc_file
		if (basename($existing_uploaded_udc_file->user_defined_compound_filepath) == 'NULL' || basename($existing_uploaded_udc_file->user_defined_compound_filepath) == '')
		{
			$udcfilename = 'No file uploaded';
		} //basename($existing_uploaded_udc_file->user_defined_compound_filepath) == 'NULL' || basename($existing_uploaded_udc_file->user_defined_compound_filepath) == ''
		else
		{
			$udcfilename = basename($existing_uploaded_udc_file->user_defined_compound_filepath);
		}
		$form['upload_an_udc'] = array(
			'#type' => 'file',
			'#title' => t('Upload an user defiend compound.'),
			'#description' => t('<span style="color:red;">Current File :</span> ' . $udcfilename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('circuit_simulation_user_defind_compound_source_extensions', '') . '</span>'
		);
		/////////////////////////////
	} //$num_of_user_defined_compounds_results != 0
	else
	{
		if (empty($form_state['num_user_defined_compounds']))
		{
			$form_state['num_user_defined_compounds'] = 1;
		} //empty($form_state['num_user_defined_compounds'])
		$temp = 0;
		for ($i = 0; $i < $form_state['num_user_defined_compounds']; $i++)
		{
			$temp = $i;
			$form['user_defined_compound_fieldset'][$i]["s_text"] = array(
				"#type" => "item",
				"#markup" => "<h4><label>User defined compounds : " . ($temp + 1) . "</label></h4>"
			);
			$form['user_defined_compound_fieldset'][$i]["udc_id"] = array(
				"#type" => "hidden",
				"#default_value" => ""
			);
			$form['user_defined_compound_fieldset'][$i]["user_defined_compound"] = array(
				"#type" => "textfield",
				"#title" => "Name of the user defined compound",
				"#default_value" => ""
			);
			$form['user_defined_compound_fieldset'][$i]["cas_no"] = array(
				"#type" => "textfield",
				"#title" => "CAS No.",
				"#default_value" => ""
			);
		} //$i = 0; $i < $form_state['num_user_defined_compounds']; $i++
		$form['user_defined_compound_fieldset']["user_defined_compound_count"] = array(
			"#type" => "hidden",
			"#value" => $temp
		);
		$form['user_defined_compound_fieldset']['add_user_defined_compounds'] = array(
			'#type' => 'submit',
			'#value' => t('Add more compounds'),
			'#limit_validation_errors' => array(),
			'#submit' => array(
				'user_defined_compounds_add_more_add_one'
			),
			'#ajax' => array(
				'callback' => 'user_defined_compounds_add_more_callback',
				'wrapper' => 'user-defined-compounds-fieldset-wrapper'
			)
		);
		if ($form_state['num_user_defined_compounds'] > 1)
		{
			$form['user_defined_compound_fieldset']['remove_user_defined_compounds'] = array(
				'#type' => 'submit',
				'#value' => t('Remove compounds'),
				'#limit_validation_errors' => array(),
				'#submit' => array(
					'user_defined_compounds_add_more_remove_one'
				),
				'#ajax' => array(
					'callback' => 'user_defined_compounds_add_more_remove_one',
					'wrapper' => 'user-defined-compounds-fieldset-wrapper'
				)
			);
		} //$form_state['num_user_defined_compounds'] > 1
		$existing_uploaded_udc_file = default_value_for_uploaded_files("UDC", $proposal_data->id);
		if (!$existing_uploaded_udc_file)
		{
			$existing_uploaded_udc_file = new stdClass();
			$existing_uploaded_udc_file->filename = "No file uploaded";
		} //!$existing_uploaded_udc_file
		if (basename($existing_uploaded_udc_file->user_defined_compound_filepath) == 'NULL' || basename($existing_uploaded_udc_file->user_defined_compound_filepath) == '')
		{
			$udcfilename = 'No file uploaded';
		} //basename($existing_uploaded_udc_file->user_defined_compound_filepath) == 'NULL' || basename($existing_uploaded_udc_file->user_defined_compound_filepath) == ''
		else
		{
			$udcfilename = basename($existing_uploaded_udc_file->user_defined_compound_filepath);
		}
		$form['upload_an_udc'] = array(
			'#type' => 'file',
			'#title' => t('Upload an user defiend compound.'),
			'#description' => t('<span style="color:red;">Current File :</span> ' . $udcfilename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('circuit_simulation_user_defind_compound_source_extensions', '') . '</span>'
		);
		if ($no_js_use)
		{
			if (!empty($form['user_defined_compound_fieldset']['remove_user_defined_compounds']['#ajax']))
			{
				unset($form['user_defined_compound_fieldset']['remove_user_defined_compounds']['#ajax']);
			} //!empty($form['user_defined_compound_fieldset']['remove_user_defined_compounds']['#ajax'])
			unset($form['user_defined_compound_fieldset']['add_user_defined_compounds']['#ajax']);
		} //$no_js_use
	}
	//////////////////////////////////////////////////////
	$existing_uploaded_A_file = default_value_for_uploaded_files("A", $proposal_data->id);
	if (!$existing_uploaded_A_file)
	{
		$existing_uploaded_A_file = new stdClass();
		$existing_uploaded_A_file->filename = "No file uploaded";
	} //!$existing_uploaded_A_file
	$form['upload_an_abstract'] = array(
		'#type' => 'file',
		'#title' => t('Upload an abstract (brief outline) of the project.'),
		'#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_A_file->filename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('circuit_simulation_abstract_upload_extensions', '') . '</span>'
	);
	$existing_uploaded_S_file = default_value_for_uploaded_files("S", $proposal_data->id);
	if (!$existing_uploaded_S_file)
	{
		$existing_uploaded_S_file = new stdClass();
		$existing_uploaded_S_file->filename = "No file uploaded";
	} //!$existing_uploaded_S_file
	$form['upload_circuit_simulation_developed_process'] = array(
		'#type' => 'file',
		'#title' => t('Upload the eSim circuit simulation for the developed process.'),
		'#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_S_file->filename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('circuit_simulation_circuit_simulation_developed_process_source_extensions', '') . '</span>'
	);
	$form['prop_id'] = array(
		'#type' => 'hidden',
		'#value' => $proposal_data->id
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#submit' => array(
			'circuit_simulation_upload_abstract_code_form_submit'
		)
	);
	$form['cancel'] = array(
		'#type' => 'item',
		'#markup' => l(t('Cancel'), 'circuit-simulation-project/manage-proposal')
	);
	return $form;
}
////////////////////////////////
function user_defined_compounds_add_more_callback($form, $form_state)
{
	return $form['user_defined_compound_fieldset'];
}
function user_defined_compounds_add_more_add_one($form, &$form_state)
{
	$form_state['num_user_defined_compounds']++;
	$form_state['rebuild'] = TRUE;
	//$form_state['no_redirect'] = TRUE;
}
function user_defined_compounds_add_more_remove_one($form, &$form_state)
{
	if ($form_state['num_user_defined_compounds'] > 1)
	{
		$form_state['num_user_defined_compounds']--;
	} //$form_state['num_user_defined_compounds'] > 1
	$form_state['rebuild'] = TRUE;
}
///////////////////////////////////////////////////////////////
function circuit_simulation_upload_abstract_code_form_validate($form, &$form_state)
{
	if ($form_state['values']['unit_operations_used_in_esim'])
	{
		$unit_operations_used_in_esim = implode(", ", $_POST['unit_operations_used_in_esim']);
		$form_state['values']['unit_operations_used_in_esim'] = $unit_operations_used_in_esim;
	} //$form_state['values']['unit_operations_used_in_esim']
	else
	{
		form_set_error('unit_operations_used_in_esim', t('Please select.'));
	}
	if ($form_state['values']['thermodynamic_packages_used'])
	{
		$thermodynamic_packages_used = implode(", ", $_POST['thermodynamic_packages_used']);
		$form_state['values']['thermodynamic_packages_used'] = $thermodynamic_packages_used;
	} //$form_state['values']['thermodynamic_packages_used']
	else
	{
		form_set_error('thermodynamic_packages_used', t('Please select.'));
	}
	if ($form_state['values']['logical_blocks_used'] != "")
	{
		$logical_blocks_used_in = $_POST['logical_blocks_used'];
		if ($logical_blocks_used_in != "")
		{
			if ($logical_blocks_used_in)
			{
				$logical_blocks_used = implode(", ", $logical_blocks_used_in);
				$form_state['values']['logical_blocks_used'] = $logical_blocks_used;
			} //$form_state['values']['logical_blocks_used']
		} //$logical_blocks_used_in != ""
		else
		{
			$form_state['values']['logical_blocks_used'] = "Not entered";
		}
	} //$form_state['values']['logical_blocks_used']
	else
	{
		$form_state['values']['logical_blocks_used'] = "Not entered";
	}
	if ($form_state['values']['list_of_compounds_from_esim_database_used_in_process_circuit_simulation'])
	{
		$list_of_compounds_from_esim_database_used_in_process_circuit_simulation = implode("| ", $_POST['list_of_compounds_from_esim_database_used_in_process_circuit_simulation']);
		$form_state['values']['list_of_compounds_from_esim_database_used_in_process_circuit_simulation'] = $list_of_compounds_from_esim_database_used_in_process_circuit_simulation;
	} //$form_state['values']['list_of_compounds_from_esim_database_used_in_process_circuit_simulation']
	if (isset($_FILES['files']))
	{
		/* check if file is uploaded */
		$existing_uploaded_A_file = default_value_for_uploaded_files("A", $form_state['values']['prop_id']);
		$existing_uploaded_S_file = default_value_for_uploaded_files("S", $form_state['values']['prop_id']);
		$existing_uploaded_udc_file = default_value_for_uploaded_files("UDC", $form_state['values']['prop_id']);
		if (!$existing_uploaded_S_file)
		{
			if (!($_FILES['files']['name']['upload_circuit_simulation_developed_process']))
				form_set_error('upload_circuit_simulation_developed_process', t('Please upload the file.'));
		} //!$existing_uploaded_S_file
		if (!$existing_uploaded_A_file)
		{
			if (!($_FILES['files']['name']['upload_an_abstract']))
				form_set_error('upload_an_abstract', t('Please upload the file.'));
		} //!$existing_uploaded_A_file
		if (!$existing_uploaded_udc_file)
		{
			if (!($_FILES['files']['name']['upload_an_udc']))
				form_set_error('upload_an_udc', t('Please upload the file.'));
		} //!$existing_uploaded_udc_file
		/* check for valid filename extensions */
		if ($_FILES['files']['name']['upload_an_udc'] || $_FILES['files']['name']['upload_an_abstract'] || $_FILES['files']['name']['upload_circuit_simulation_developed_process'])
		{
			foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
			{
				if ($file_name)
				{
					/* checking file type */
					if (strstr($file_form_name, 'upload_circuit_simulation_developed_process'))
						$file_type = 'S';
					else if (strstr($file_form_name, 'upload_an_abstract'))
						$file_type = 'A';
					else if (strstr($file_form_name, 'upload_an_udc'))
						$file_type = 'UDC';
					else
						$file_type = 'U';
					$allowed_extensions_str = '';
					switch ($file_type)
					{
						case 'S':
							$allowed_extensions_str = variable_get('circuit_simulation_circuit_simulation_developed_process_source_extensions', '');
							break;
						case 'A':
							$allowed_extensions_str = variable_get('circuit_simulation_abstract_upload_extensions', '');
							break;
						case 'UDC':
							$allowed_extensions_str = variable_get('circuit_simulation_user_defind_compound_source_extensions', '');
							break;
					} //$file_type
					$allowed_extensions = explode(',', $allowed_extensions_str);
					$tmp_ext = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
					$temp_extension = end($tmp_ext);
					if (!in_array($temp_extension, $allowed_extensions))
						form_set_error($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
					if ($_FILES['files']['size'][$file_form_name] <= 0)
						form_set_error($file_form_name, t('File size cannot be zero.'));
					/* check if valid file name */
					if (!circuit_simulation_check_valid_filename($_FILES['files']['name'][$file_form_name]))
						form_set_error($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
				} //$file_name
			} //$_FILES['files']['name'] as $file_form_name => $file_name
		} //$_FILES['files']['name'] as $file_form_name => $file_name
	} //isset($_FILES['files'])
	// drupal_add_js('jQuery(document).ready(function () { alert("Hello!"); });', 'inline');
	// drupal_static_reset('drupal_add_js') ;
}
function circuit_simulation_upload_abstract_code_form_submit($form, &$form_state)
{
	global $user;
	$v = $form_state['values'];
	$root_path = circuit_simulation_path();
	$proposal_data = circuit_simulation_get_proposal();
	$proposal_id = $proposal_data->id;
	if (!$proposal_data)
	{
		drupal_goto('');
		return;
	} //!$proposal_data
	$proposal_id = $proposal_data->id;
	$proposal_directory = $proposal_data->directory_name;
	/* create proposal folder if not present */
	$dest_path = $proposal_directory . '/';
	$dest_path_udc = $proposal_directory . '/user_defined_compound/';
	if (!is_dir($root_path . $dest_path))
		mkdir($root_path . $dest_path);
	if ($proposal_data)
	{
		$query = "UPDATE {circuit_simulation_proposal} SET 

	esim_database_compound_name =  :esim_database_compound_name
 
	WHERE id = :proposal_id
	";
		$args = array(
			":esim_database_compound_name" => $v['list_of_compounds_from_esim_database_used_in_process_circuit_simulation'],
			":proposal_id" => $proposal_id
		);
		$submitted_proposal_id = db_query($query, $args, array(
			'return' => Database::RETURN_INSERT_ID
		));
	} //$proposal_data
	$proposal_id = $proposal_data->id;
	$query_s = "SELECT * FROM {circuit_simulation_submitted_abstracts} WHERE proposal_id = :proposal_id";
	$args_s = array(
		":proposal_id" => $proposal_id
	);
	$query_s_result = db_query($query_s, $args_s)->fetchObject();
	if (!$query_s_result)
	{
		/* creating solution database entry */
		$query = "INSERT INTO {circuit_simulation_submitted_abstracts} (
	proposal_id,
	approver_uid,
	abstract_approval_status,
	unit_operations_used_in_esim,
	thermodynamic_packages_used,
	logical_blocks_used,
	abstract_upload_date,
	abstract_approval_date,
	is_submitted) VALUES (:proposal_id, :approver_uid, :abstract_approval_status, :unit_operations_used_in_esim, 
  :thermodynamic_packages_used, :logical_blocks_used, :abstract_upload_date, :abstract_approval_date, :is_submitted)";
		$args = array(
			":proposal_id" => $proposal_id,
			":approver_uid" => 0,
			":abstract_approval_status" => 0,
			":unit_operations_used_in_esim" => $v['unit_operations_used_in_esim'],
			":thermodynamic_packages_used" => $v['thermodynamic_packages_used'],
			":logical_blocks_used" => $v['logical_blocks_used'],
			":abstract_upload_date" => time(),
			":abstract_approval_date" => 0,
			":is_submitted" => 0
		);
		$submitted_abstract_id = db_query($query, $args, array(
			'return' => Database::RETURN_INSERT_ID
		));
		drupal_set_message('Abstract uploaded successfully.', 'status');
	} //!$query_s_result
	else
	{
		$query = "UPDATE {circuit_simulation_submitted_abstracts} SET 

	unit_operations_used_in_esim=  :unit_operations_used_in_esim,
	thermodynamic_packages_used= :thermodynamic_packages_used,
	logical_blocks_used=:logical_blocks_used,
	abstract_upload_date =:abstract_upload_date,
	is_submitted= :is_submitted 
	WHERE proposal_id = :proposal_id
	";
		$args = array(
			":unit_operations_used_in_esim" => $v['unit_operations_used_in_esim'],
			":thermodynamic_packages_used" => $v['thermodynamic_packages_used'],
			":logical_blocks_used" => $v['logical_blocks_used'],
			":abstract_upload_date" => time(),
			":is_submitted" => 0,
			":proposal_id" => $proposal_id
		);
		$submitted_abstract_id = db_query($query, $args, array(
			'return' => Database::RETURN_INSERT_ID
		));
		drupal_set_message('Abstract updated successfully.', 'status');
	}
	// For editing user defiend compounds
	$user_defined_compoundupload = 0;
	for ($i = 0; $i <= $v['user_defined_compound_fieldset']["user_defined_compound_count"]; $i++)
	{
		$udc_id = $v['user_defined_compound_fieldset'][$i]["udc_id"];
		if ($udc_id != "")
		{
			if ($v['user_defined_compound_fieldset'][$i]["user_defined_compound"] != "")
			{
				$query = db_update('circuit_simulation_user_defined_compound');
				$query->fields(array(
					'user_defined_compound' => $v['user_defined_compound_fieldset'][$i]["user_defined_compound"],
					'cas_no' => $v['user_defined_compound_fieldset'][$i]["cas_no"]
				));
				$query->condition('id', $v['user_defined_compound_fieldset'][$i]["udc_id"]);
				$result = $query->execute();
				if ($result != 0)
				{
					$user_defined_compoundupload++;
				} //$result != 0
			} //$v['user_defined_compound_fieldset'][$i]["user_defined_compound"] != ""
		} //$udc_id != ""
		else
		{
			if ($v['user_defined_compound_fieldset'][$i]["user_defined_compound"] != "")
			{
				$user_defined_compoundquery = "
	INSERT INTO circuit_simulation_user_defined_compound
	(proposal_id,user_defined_compound,cas_no)
	VALUES
	(:proposal_id,:user_defined_compound,:cas_no)
	";
				$user_defined_compoundargs = array(
					":proposal_id" => $proposal_id,
					":user_defined_compound" => $v['user_defined_compound_fieldset'][$i]["user_defined_compound"],
					":cas_no" => $v['user_defined_compound_fieldset'][$i]["cas_no"]
				);
				/* storing the row id in $result */
				$user_defined_compoundresult = db_query($user_defined_compoundquery, $user_defined_compoundargs, array(
					'return' => Database::RETURN_INSERT_ID
				));
				if ($user_defined_compoundresult != 0)
				{
					$user_defined_compoundupload++;
				} //$user_defined_compoundresult != 0
			} //$v['user_defined_compound_fieldset'][$i]["user_defined_compound"] != ""
		}
	} //$i = 0; $i <= $v["user_defined_compound_count"]; $i++
	/* uploading files */
	foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
	{
		if ($file_name)
		{
			/* checking file type */
			if (strstr($file_form_name, 'upload_circuit_simulation_developed_process'))
			{
				$file_type = 'S';
			} //strstr($file_form_name, 'upload_circuit_simulation_developed_process')
			else if (strstr($file_form_name, 'upload_an_abstract'))
			{
				$file_type = 'A';
			} //strstr($file_form_name, 'upload_an_abstract')
			else if (strstr($file_form_name, 'upload_an_udc'))
			{
				$file_type = 'UDC';
			} //strstr($file_form_name, 'upload_an_udc')
			else
			{
				$file_type = 'U';
			}
			switch ($file_type)
			{
				case 'S':
					if (file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
					{
						//unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
						drupal_set_message(t("File !filename already exists hence overwirtten the exisitng file ", array(
							'!filename' => $_FILES['files']['name'][$file_form_name]
						)), 'error');
					} //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
					/* uploading file */
					else if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
					{
						/* for uploaded files making an entry in the database */
						$query_ab_f = "SELECT * FROM circuit_simulation_submitted_abstracts_file WHERE proposal_id = :proposal_id AND filetype = 
				:filetype";
						$args_ab_f = array(
							":proposal_id" => $proposal_id,
							":filetype" => $file_type
						);
						$query_ab_f_result = db_query($query_ab_f, $args_ab_f)->fetchObject();
						if (!$query_ab_f_result)
						{
							$query = "INSERT INTO {circuit_simulation_submitted_abstracts_file} (submitted_abstract_id, proposal_id, uid, approvar_uid, filename, filepath, filemime, filesize, filetype, timestamp)
          VALUES (:submitted_abstract_id, :proposal_id, :uid, :approvar_uid, :filename, :filepath, :filemime, :filesize, :filetype, :timestamp)";
							$args = array(
								":submitted_abstract_id" => $submitted_abstract_id,
								":proposal_id" => $proposal_id,
								":uid" => $user->uid,
								":approvar_uid" => 0,
								":filename" => $_FILES['files']['name'][$file_form_name],
								":filepath" => $_FILES['files']['name'][$file_form_name],
								":filemime" => mime_content_type($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]),
								":filesize" => $_FILES['files']['size'][$file_form_name],
								":filetype" => $file_type,
								":timestamp" => time()
							);
							db_query($query, $args);
							drupal_set_message($file_name . ' uploaded successfully.', 'status');
						} //!$query_ab_f_result
						else
						{
							unlink($root_path . $dest_path . $query_ab_f_result->filename);
							$query = "UPDATE {circuit_simulation_submitted_abstracts_file} SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id AND filetype = :filetype";
							$args = array(
								":filename" => $_FILES['files']['name'][$file_form_name],
								":filepath" => $file_path . $_FILES['files']['name'][$file_form_name],
								":filemime" => mime_content_type($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]),
								":filesize" => $_FILES['files']['size'][$file_form_name],
								":timestamp" => time(),
								":proposal_id" => $proposal_id,
								":filetype" => $file_type
							);
							db_query($query, $args);
							drupal_set_message($file_name . ' file updated successfully.', 'status');
						}
					} //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
					else
					{
						drupal_set_message('Error uploading file : ' . $dest_path . $file_name, 'error');
					}
					break;
				case 'A':
					if (file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
					{
						//unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
						drupal_set_message(t("File !filename already exists hence overwirtten the exisitng file ", array(
							'!filename' => $_FILES['files']['name'][$file_form_name]
						)), 'error');
					} //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
					/* uploading file */
					else if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
					{
						/* for uploaded files making an entry in the database */
						$query_ab_f = "SELECT * FROM circuit_simulation_submitted_abstracts_file WHERE proposal_id = :proposal_id AND filetype = 
				:filetype";
						$args_ab_f = array(
							":proposal_id" => $proposal_id,
							":filetype" => $file_type
						);
						$query_ab_f_result = db_query($query_ab_f, $args_ab_f)->fetchObject();
						if (!$query_ab_f_result)
						{
							$query = "INSERT INTO {circuit_simulation_submitted_abstracts_file} (submitted_abstract_id, proposal_id, uid, approvar_uid, filename, filepath, filemime, filesize, filetype, timestamp)
          VALUES (:submitted_abstract_id, :proposal_id, :uid, :approvar_uid, :filename, :filepath, :filemime, :filesize, :filetype, :timestamp)";
							$args = array(
								":submitted_abstract_id" => $submitted_abstract_id,
								":proposal_id" => $proposal_id,
								":uid" => $user->uid,
								":approvar_uid" => 0,
								":filename" => $_FILES['files']['name'][$file_form_name],
								":filepath" => $_FILES['files']['name'][$file_form_name],
								":filemime" => mime_content_type($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]),
								":filesize" => $_FILES['files']['size'][$file_form_name],
								":filetype" => $file_type,
								":timestamp" => time()
							);
							db_query($query, $args);
							drupal_set_message($file_name . ' uploaded successfully.', 'status');
						} //!$query_ab_f_result
						else
						{
							unlink($root_path . $dest_path . $query_ab_f_result->filename);
							$query = "UPDATE {circuit_simulation_submitted_abstracts_file} SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id AND filetype = :filetype";
							$args = array(
								":filename" => $_FILES['files']['name'][$file_form_name],
								":filepath" => $file_path . $_FILES['files']['name'][$file_form_name],
								":filemime" => mime_content_type($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]),
								":filesize" => $_FILES['files']['size'][$file_form_name],
								":timestamp" => time(),
								":proposal_id" => $proposal_id,
								":filetype" => $file_type
							);
							db_query($query, $args);
							drupal_set_message($file_name . ' file updated successfully.', 'status');
						}
					} //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
					else
					{
						drupal_set_message('Error uploading file : ' . $dest_path . $file_name, 'error');
					}
					break;
				case 'UDC':
					if (!is_dir($root_path . $dest_path_udc))
						mkdir($root_path . $dest_path_udc);
					if (file_exists($root_path . $dest_path_udc . $_FILES['files']['name'][$file_form_name]))
					{
						unlink($root_path . $dest_path_udc . $_FILES['files']['name'][$file_form_name]);
						drupal_set_message(t("File !filename already exists directory hence overwirtten the exisitng file ", array(
							'!filename' => $_FILES['files']['name'][$file_form_name]
						)), 'error');
					} //file_exists($root_path . $dest_path_udc . $_FILES['files']['name'][$file_form_name])
					if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path_udc . $_FILES['files']['name'][$file_form_name]))
					{
						//////////////////////////////////
						/* for uploaded files making an entry in the database */
						$query_udc_f = "SELECT * FROM circuit_simulation_proposal WHERE id = :proposal_id";
						$args_udc_f = array(
							":proposal_id" => $proposal_id
						);
						$query_udc_f_result = db_query($query_udc_f, $args_udc_f)->fetchObject();
						if ($query_udc_f_result)
						{
							unlink($root_path . $dest_path_udc . $query_ab_f_result->user_defined_compound_filepath);
							$user_defined_compound_filepath = "user_defined_compound/" . $_FILES['files']['name'][$file_form_name];
							$query_udc_f = "UPDATE circuit_simulation_proposal SET user_defined_compound_filepath = :user_defined_compound_filepath WHERE id= :proposal_id";
							$args_udc_f = array(
								":user_defined_compound_filepath" => $user_defined_compound_filepath,
								":proposal_id" => $proposal_id
							);
							db_query($query_udc_f, $args_udc_f);
							drupal_set_message($file_name . ' uploaded successfully.', 'status');
						} //!$query_ab_f_result
						else
						{
							drupal_set_message('Invalid proposal', 'error');
						}
						//////////////////////////////////
					} //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path_udc . $_FILES['files']['name'][$file_form_name])
					else
					{
						drupal_set_message($file_name . " unable to move.");
					}
					break;
			} //$file_type
		} //$file_name
	} //$_FILES['files']['name'] as $file_form_name => $file_name
	/* sending email */
	$email_to = $user->mail;
	$from = variable_get('circuit_simulation_from_email', '');
	$bcc = variable_get('circuit_simulation_emails', '');
	$cc = variable_get('circuit_simulation_cc_emails', '');
	$params['abstract_uploaded']['proposal_id'] = $proposal_id;
	$params['abstract_uploaded']['submitted_abstract_id'] = $submitted_abstract_id;
	$params['abstract_uploaded']['user_id'] = $user->uid;
	$params['abstract_uploaded']['headers'] = array(
		'From' => $from,
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
		'Cc' => $cc,
		'Bcc' => $bcc
	);
	if (!drupal_mail('circuit_simulation', 'abstract_uploaded', $email_to, language_default(), $params, $from, TRUE))
		drupal_set_message('Error sending email message.', 'error');
	drupal_goto('circuit-simulation-project/abstract-code');
}
function default_value_for_selections($opration, $proposal_id)
{
	$query = db_select('circuit_simulation_submitted_abstracts');
	$query->fields('circuit_simulation_submitted_abstracts');
	$query->condition('proposal_id', $proposal_id);
	$abstracts_q = $query->execute()->fetchObject();
	$selected_pacakege_array = array();
	if ($opration == "unit_operations_used_in_esim")
	{
		$uouid = explode(',', $abstracts_q->unit_operations_used_in_esim);
		$ui = 0;
		$unit_item = new stdClass();
		foreach ($uouid as $unit_item->$ui)
		{
			$selected_pacakege_array[$ui] = trim($unit_item->$ui);
			$ui++;
		} //$uouid as $unit_item->$ui
	} //$opration == "unit_operations_used_in_esim"
	elseif ($opration == "thermodynamic_packages_used")
	{
		$tpuid = explode(',', $abstracts_q->thermodynamic_packages_used);
		$tpui = 0;
		$thermodynamic_item = new stdClass();
		foreach ($tpuid as $thermodynamic_item->$tpui)
		{
			$selected_pacakege_array[$tpui] = trim($thermodynamic_item->$tpui);
			$tpui++;
		} //$tpuid as $thermodynamic_item->$tpui
	} //$opration == "thermodynamic_packages_used"
	elseif ($opration == "logical_blocks_used")
	{
		$lbuid = explode(',', $abstracts_q->logical_blocks_used);
		$lbui = 0;
		$logical_blocks = new stdClass();
		foreach ($lbuid as $logical_blocks->$lbui)
		{
			$selected_pacakege_array[$logical_blocks->$lbui] = trim($logical_blocks->$lbui);
			$lbui++;
		} //$lbuid as $logical_blocks->$lbui
	} //$opration == "logical_blocks_used"
	elseif ($opration == "esim_database_compound_name")
	{
		$esim_database_compound_name = new stdClass();
		$query = db_select('circuit_simulation_proposal');
		$query->fields('circuit_simulation_proposal');
		$query->condition('id', $proposal_id);
		$proposal_q = $query->execute()->fetchObject();
		$ddcm = explode('| ', $proposal_q->esim_database_compound_name);
		$ddcmi = 0;
		foreach ($ddcm as $esim_database_compound_name->$ddcmi)
		{
			$selected_pacakege_array[$esim_database_compound_name->$ddcmi] = trim($esim_database_compound_name->$ddcmi);
			$ddcmi++;
		} //$ddcm as $esim_database_compound_name->$ddcmi
	} //$opration == "esim_database_compound_name"
	else
	{
		return $selected_pacakege_array;
	}
	return $selected_pacakege_array;
}
function default_value_for_uploaded_files($filetype, $proposal_id)
{
	$query = db_select('circuit_simulation_submitted_abstracts_file');
	$query->fields('circuit_simulation_submitted_abstracts_file');
	$query->condition('proposal_id', $proposal_id);
	$selected_files_array = "";
	if ($filetype == "A")
	{
		$query->condition('filetype', $filetype);
		$filetype_q = $query->execute()->fetchObject();
		return $filetype_q;
	} //$filetype == "A"
	elseif ($filetype == "S")
	{
		$query->condition('filetype', $filetype);
		$filetype_q = $query->execute()->fetchObject();
		return $filetype_q;
	} //$filetype == "S"
	elseif ($filetype == "UDC")
	{
		$query = db_select('circuit_simulation_proposal');
		$query->fields('circuit_simulation_proposal');
		$query->condition('id', $proposal_id);
		$filetype_q = $query->execute()->fetchObject();
		return $filetype_q;
	} //$filetype == "S"
	else
	{
		return;
	}
	return;
}
