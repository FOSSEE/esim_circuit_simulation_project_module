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
			//$abstract_filename = l($abstracts_pdf->filename, 'circuit-simulation-project/download/project-file/' . $proposal_data->id);
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
			//$abstracts_query_process_filename = l($abstracts_query_process->filename, 'circuit-simulation-project/download/project-file/' . $proposal_data->id); 
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
			$url = l('Edit', 'circuit-simulation-project/abstract-code/upload');
		} //$abstracts_q->is_submitted == 0
	} //$abstracts_query_process == TRUE
	else
	{
		$url = l('Upload abstract', 'circuit-simulation-project/abstract-code/upload');
		$abstracts_query_process_filename = "File not uploaded";
	}
	$return_html .= '<strong>Contributor Name:</strong><br />' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '<br /><br />';
	$return_html .= '<strong>Title of the Circuit Simulation Project:</strong><br />' . $proposal_data->project_title . '<br /><br />';
	$return_html .= '<strong>Uploaded abstract of the project:</strong><br />' . $abstract_filename . '<br /><br />';
	$return_html .= '<strong>Uploaded project files:</strong><br />' . $abstracts_query_process_filename . '<br /><br />';
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
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
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
	$query = db_select('esim_circuit_simulation_submitted_abstracts');
	$query->fields('esim_circuit_simulation_submitted_abstracts');
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
		'#title' => t('Title of the Circuit Simulation Project')
	);
	$form['contributor_name'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->contributor_name,
		'#title' => t('Contributor Name')
	);
	$existing_uploaded_A_file = default_value_for_uploaded_files("A", $proposal_data->id);
	if (!$existing_uploaded_A_file)
	{
		$existing_uploaded_A_file = new stdClass();
		$existing_uploaded_A_file->filename = "No file uploaded";
	} //!$existing_uploaded_A_file
	$form['upload_an_abstract'] = array(
		'#type' => 'file',
		'#title' => t('Upload an abstract of the project.') ,
		'#description' => t('<span style="font-size:16px;"><a href="http://static.fossee.in/esim/manuals/Analysis_of_frequency_response_of_the_BJT_amplifier_using_eSim_with_Python_Plots.pdf">For a sample click here</a></span>' . '<br />' . t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_A_file->filename . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('circuit_simulation_abstract_upload_extensions', '') . '</span>'
	)));
	$existing_uploaded_S_file = default_value_for_uploaded_files("S", $proposal_data->id);
	if (!$existing_uploaded_S_file)
	{
		$existing_uploaded_S_file = new stdClass();
		$existing_uploaded_S_file->filename = "No file uploaded";
	} //!$existing_uploaded_S_file
	$form['upload_circuit_simulation_developed_process'] = array(
		'#type' => 'file',
		'#title' => t('Upload the Project Files'),
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

///////////////////////////////////////////////////////////////
function circuit_simulation_upload_abstract_code_form_validate($form, &$form_state)
{
	if (isset($_FILES['files']))
	{
		/* check if file is uploaded */
		$existing_uploaded_A_file = default_value_for_uploaded_files("A", $form_state['values']['prop_id']);
		$existing_uploaded_S_file = default_value_for_uploaded_files("S", $form_state['values']['prop_id']);
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
	//$dest_path = $proposal_directory . '/';
	$dest_path_project_files = $proposal_directory . '/project_files/';
	if (!is_dir($root_path . $dest_path_project_files))
		mkdir($root_path . $dest_path_project_files);
	$proposal_id = $proposal_data->id;
	$query_s = "SELECT * FROM {esim_circuit_simulation_submitted_abstracts} WHERE proposal_id = :proposal_id";
	$args_s = array(
		":proposal_id" => $proposal_id
	);
	$query_s_result = db_query($query_s, $args_s)->fetchObject();
	if (!$query_s_result)
	{
		/* creating solution database entry */
		$query = "INSERT INTO {esim_circuit_simulation_submitted_abstracts} (
	proposal_id,
	approver_uid,
	abstract_approval_status,
	abstract_upload_date,
	abstract_approval_date,
	is_submitted) VALUES (:proposal_id, :approver_uid, :abstract_approval_status,:abstract_upload_date, :abstract_approval_date, :is_submitted)";
		$args = array(
			":proposal_id" => $proposal_id,
			":approver_uid" => 0,
			":abstract_approval_status" => 0,
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
		$query = "UPDATE {esim_circuit_simulation_submitted_abstracts} SET 

	
	abstract_upload_date =:abstract_upload_date,
	is_submitted= :is_submitted 
	WHERE proposal_id = :proposal_id
	";
		$args = array(
			
			":abstract_upload_date" => time(),
			":is_submitted" => 0,
			":proposal_id" => $proposal_id
		);
		$submitted_abstract_id = db_query($query, $args, array(
			'return' => Database::RETURN_INSERT_ID
		));
		drupal_set_message('Abstract updated successfully.', 'status');
	}
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
			} 
			else
			{
				$file_type = 'U';
			}
			switch ($file_type)
			{
				case 'S':
					if (file_exists($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]))
					{
						//unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
						drupal_set_message(t("File !filename already exists hence overwirtten the exisitng file ", array(
							'!filename' => $_FILES['files']['name'][$file_form_name]
						)), 'error');
					} //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
					/* uploading file */
					else if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]))
					{
						/* for uploaded files making an entry in the database */
						$query_ab_f = "SELECT * FROM esim_circuit_simulation_submitted_abstracts_file WHERE proposal_id = :proposal_id AND filetype = 
				:filetype";
						$args_ab_f = array(
							":proposal_id" => $proposal_id,
							":filetype" => $file_type
						);
						$query_ab_f_result = db_query($query_ab_f, $args_ab_f)->fetchObject();
						if (!$query_ab_f_result)
						{
							$query = "INSERT INTO {esim_circuit_simulation_submitted_abstracts_file} (submitted_abstract_id, proposal_id, uid, approvar_uid, filename, filepath, filemime, filesize, filetype, timestamp)
          VALUES (:submitted_abstract_id, :proposal_id, :uid, :approvar_uid, :filename, :filepath, :filemime, :filesize, :filetype, :timestamp)";
							$args = array(
								":submitted_abstract_id" => $submitted_abstract_id,
								":proposal_id" => $proposal_id,
								":uid" => $user->uid,
								":approvar_uid" => 0,
								":filename" => $_FILES['files']['name'][$file_form_name],
								":filepath" => $_FILES['files']['name'][$file_form_name],
								":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
								":filesize" => $_FILES['files']['size'][$file_form_name],
								":filetype" => $file_type,
								":timestamp" => time()
							);
							db_query($query, $args);
							drupal_set_message($file_name . ' uploaded successfully.', 'status');
						} //!$query_ab_f_result
						else
						{
							unlink($root_path . $dest_path_project_files . $query_ab_f_result->filename);
							$query = "UPDATE {esim_circuit_simulation_submitted_abstracts_file} SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id AND filetype = :filetype";
							$args = array(
								":filename" => $_FILES['files']['name'][$file_form_name],
								":filepath" => $file_path . $_FILES['files']['name'][$file_form_name],
								":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
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
						drupal_set_message('Error uploading file : ' . $dest_path_project_files . $file_name, 'error');
					}
					break;
				case 'A':
					if (file_exists($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]))
					{
						//unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
						drupal_set_message(t("File !filename already exists hence overwirtten the exisitng file ", array(
							'!filename' => $_FILES['files']['name'][$file_form_name]
						)), 'error');
					} //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
					/* uploading file */
					else if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]))
					{
						/* for uploaded files making an entry in the database */
						$query_ab_f = "SELECT * FROM esim_circuit_simulation_submitted_abstracts_file WHERE proposal_id = :proposal_id AND filetype = 
				:filetype";
						$args_ab_f = array(
							":proposal_id" => $proposal_id,
							":filetype" => $file_type
						);
						$query_ab_f_result = db_query($query_ab_f, $args_ab_f)->fetchObject();
						if (!$query_ab_f_result)
						{
							$query = "INSERT INTO {esim_circuit_simulation_submitted_abstracts_file} (submitted_abstract_id, proposal_id, uid, approvar_uid, filename, filepath, filemime, filesize, filetype, timestamp)
          VALUES (:submitted_abstract_id, :proposal_id, :uid, :approvar_uid, :filename, :filepath, :filemime, :filesize, :filetype, :timestamp)";
							$args = array(
								":submitted_abstract_id" => $submitted_abstract_id,
								":proposal_id" => $proposal_id,
								":uid" => $user->uid,
								":approvar_uid" => 0,
								":filename" => $_FILES['files']['name'][$file_form_name],
								":filepath" => $_FILES['files']['name'][$file_form_name],
								":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
								":filesize" => $_FILES['files']['size'][$file_form_name],
								":filetype" => $file_type,
								":timestamp" => time()
							);
							db_query($query, $args);
							drupal_set_message($file_name . ' uploaded successfully.', 'status');
						} //!$query_ab_f_result
						else
						{
							unlink($root_path . $dest_path_project_files . $query_ab_f_result->filename);
							$query = "UPDATE {esim_circuit_simulation_submitted_abstracts_file} SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id AND filetype = :filetype";
							$args = array(
								":filename" => $_FILES['files']['name'][$file_form_name],
								":filepath" => $file_path . $_FILES['files']['name'][$file_form_name],
								":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
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
						drupal_set_message('Error uploading file : ' . $dest_path_project_files . $file_name, 'error');
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
function default_value_for_uploaded_files($filetype, $proposal_id)
{
	$query = db_select('esim_circuit_simulation_submitted_abstracts_file');
	$query->fields('esim_circuit_simulation_submitted_abstracts_file');
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
	}
	else
	{
		return;
	}
	return;
}