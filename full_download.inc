<?php
// $Id$
function circuit_simulation_download_full_project() {
	global $user;
	$circuit_simulation_id = arg(3);
	$root_path = circuit_simulation_path();
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $circuit_simulation_id);
	$circuit_simulation_q = $query->execute();
	$circuit_simulation_data = $circuit_simulation_q->fetchObject();
	$FLOWSHEET_PATH = $circuit_simulation_data->directory_name . '/';
	
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	//var_dump($zip_filename);die;
	$zip = new ZipArchive();
	//var_dump($zip);die;
    $zip->open($zip_filename, ZipArchive::CREATE);
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $circuit_simulation_id);
	$circuit_simulation_udc_q = $query->execute();

	while ($circuit_simulation_udc_row = $circuit_simulation_udc_q->fetchObject()) {
		if ($circuit_simulation_udc_row->user_defined_compound_filepath || $circuit_simulation_udc_row->user_defined_compound_filepath != 'NULL') {
			$USER_DEFINED_PATH = 'user_defined_compound/';
			
			$zip->addFile($root_path . $FLOWSHEET_PATH . '/' . $circuit_simulation_udc_row->user_defined_compound_filepath, $USER_DEFINED_PATH . str_replace(' ', '_', basename($circuit_simulation_udc_row->user_defined_compound_filepath)));
		} //$circuit_simulation_udc_row->user_defined_compound_filepath || $circuit_simulation_udc_row->user_defined_compound_filepath != 'NULL'
	} //$circuit_simulation_udc_row = $circuit_simulation_udc_q->fetchObject()
	$query = db_select('esim_circuit_simulation_submitted_abstracts_file');
	$query->fields('esim_circuit_simulation_submitted_abstracts_file');
	$query->condition('proposal_id', $circuit_simulation_id);
	$circuit_simulation_f_q = $query->execute();
	while ($circuit_simulation_f_row = $circuit_simulation_f_q->fetchObject()) {
		$zip->addFile($root_path . $FLOWSHEET_PATH . '/' . $circuit_simulation_f_row->filepath, str_replace(' ', '_', basename($circuit_simulation_f_row->filename)));
	} //$circuit_simulation_f_row = $circuit_simulation_f_q->fetchObject()
	$zip_file_count = $zip->numFiles;
	$zip->close();
	//var_dump($zip_file_count);die;
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $circuit_simulation_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $circuit_simulation_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		drupal_set_message("There are circuit_simulation project in this proposal to download", 'error');
		drupal_goto('circuit-simulation-project/full-download/project');
	}
}
function circuit_simulation_download_completed_project()
{
	global $user;
	$circuit_simulation_id = arg(3);
	$root_path = circuit_simulation_path();
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $circuit_simulation_id);
	$circuit_simulation_q = $query->execute();
	$circuit_simulation_data = $circuit_simulation_q->fetchObject();
	$FLOWSHEET_PATH = $circuit_simulation_data->directory_name . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive();
	//var_dump($zip);die;
	$zip->open($zip_filename, ZipArchive::CREATE);
	var_dump($zip);die;
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $circuit_simulation_id);
	$circuit_simulation_udc_q = $query->execute();
	while ($circuit_simulation_udc_row = $circuit_simulation_udc_q->fetchObject()) {
		if ($circuit_simulation_udc_row->user_defined_compound_filepath || $circuit_simulation_udc_row->user_defined_compound_filepath != 'NULL') {
			$USER_DEFINED_PATH = 'user_defined_compound/';
			$zip->addFile($root_path . $FLOWSHEET_PATH . '/' . $circuit_simulation_udc_row->user_defined_compound_filepath, $USER_DEFINED_PATH . str_replace(' ', '_', basename($circuit_simulation_udc_row->user_defined_compound_filepath)));
		} //$circuit_simulation_udc_row->user_defined_compound_filepath || $circuit_simulation_udc_row->user_defined_compound_filepath != 'NULL'
	} //$circuit_simulation_udc_row = $circuit_simulation_udc_q->fetchObject()
	$query = db_select('esim_circuit_simulation_submitted_abstracts_file');
	$query->fields('esim_circuit_simulation_submitted_abstracts_file');
	$query->condition('proposal_id', $circuit_simulation_id);
	$circuit_simulation_f_q = $query->execute();
	while ($circuit_simulation_f_row = $circuit_simulation_f_q->fetchObject()) {
		$zip->addFile($root_path . $FLOWSHEET_PATH . '/' . $circuit_simulation_f_row->filepath, str_replace(' ', '_', basename($circuit_simulation_f_row->filename)));
	} //$circuit_simulation_f_row = $circuit_simulation_f_q->fetchObject()
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $circuit_simulation_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $circuit_simulation_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		drupal_set_message("There are circuit_simulation project in this proposal to download", 'error');
		drupal_goto('circuit-simulation-project/esim-circuit_simulation-run');
	}
}

