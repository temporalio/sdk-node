use temporal_sdk_core::{
    protos::coresdk::{
        poll_sdk_task_resp::Task, CompleteSdkTaskReq, PollSdkTaskResp, RegistrationReq,
    },
    CoreSDKService, Result,
    SDKServiceError::Unknown,
};

#[derive(Clone)]
pub struct MockCore {
    pub tasks: ::std::collections::VecDeque<Task>,
}

impl CoreSDKService for MockCore {
    fn poll_sdk_task(&self) -> Result<PollSdkTaskResp> {
        match self.tasks.get(0) {
            Some(task) => Result::Ok(PollSdkTaskResp {
                task_token: b"abc".to_vec(),
                task: Some(task.clone()),
            }),
            _ => Result::Err(Unknown {}),
        }
    }

    #[allow(dead_code)]
    fn complete_sdk_task(&self, _req: CompleteSdkTaskReq) -> Result<()> {
        Result::Ok(())
    }

    #[allow(dead_code)]
    fn register_implementations(&self, _req: RegistrationReq) -> Result<()> {
        Result::Ok(())
    }
}
